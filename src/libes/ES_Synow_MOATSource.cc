// 
// File    : ES_Synow_MOATSource.cc
// --------------------------------
//
// Copyright (C) 2010 Rollin C. Thomas <rcthomas@lbl.gov>
// 
// This file is part of ES.
// 
// ES is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your
// option) any later version.
// 
// ES is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
// 
// You should have received a copy of the GNU General Public License along
// with ES.  If not, see <http://www.gnu.org/licenses/>.
//  

#include "ES_Synow_MOATSource.hh"
#include "ES_Synow_Grid.hh"
#include "ES_Synow_Setup.hh"
#include "ES_Blackbody.hh"

ES::Synow::MOATSource::MOATSource( ES::Synow::Grid& grid, size_t const mu_size ) :
    ES::Synow::Operator( grid ),
    _mu_size( mu_size )
{

    // Allocate CPU buffers.

    size_t v_size  = _grid->v_size;
    size_t wl_size = _grid->wl_size;
    _mu    = new double [           v_size * _mu_size * 2 ];
    _dmu   = new double [           v_size * _mu_size * 2 ];
    _shift = new double [           v_size * _mu_size * 2 ];
    _start = new size_t [ wl_size * v_size * _mu_size * 2 ];
    _in    = new double [ wl_size * v_size * _mu_size * 2 ]; 

    // Read binary programs.

    _binary = moat_ocl_binary_read( "ES_Synow_MOATSource", "-Werror" );

    // Prepare one of the GPUs.

    _dev     = moat_ocl_getgpu( 0 );
    _context = moat_ocl_getcontext( _dev );
    _queue   = _dev->outoforder ? moat_ocl_ooqueue_get( _dev ) : moat_ocl_ioqueue_get( _dev );

    // Load coarse kernel for computing the source function.

    _kernel = moat_ocl_kernel_get( _binary, _dev, "ES_Synow_MOATSource" );

    // Allocate buffers needed for source metadata initialization.

    _dev_v     = clCreateBuffer( _context, CL_MEM_READ_ONLY , sizeof( float  ) *           v_size               , NULL, NULL );
    _dev_mu    = clCreateBuffer( _context, CL_MEM_READ_ONLY , sizeof( float  ) *           v_size * _mu_size * 2, NULL, NULL );
    _dev_dmu   = clCreateBuffer( _context, CL_MEM_READ_ONLY , sizeof( float  ) *           v_size * _mu_size * 2, NULL, NULL );
    _dev_shift = clCreateBuffer( _context, CL_MEM_READ_ONLY , sizeof( float  ) *           v_size * _mu_size * 2, NULL, NULL );
    _dev_wl    = clCreateBuffer( _context, CL_MEM_READ_ONLY , sizeof( float  ) * wl_size                        , NULL, NULL );
    _dev_tau   = clCreateBuffer( _context, CL_MEM_READ_ONLY , sizeof( float  ) * wl_size * v_size               , NULL, NULL );
    _dev_start = clCreateBuffer( _context, CL_MEM_READ_ONLY , sizeof( size_t ) * wl_size * v_size * _mu_size * 2, NULL, NULL );
    _dev_in    = clCreateBuffer( _context, CL_MEM_READ_WRITE, sizeof( float  ) * wl_size * v_size * _mu_size * 2, NULL, NULL );
    _dev_src   = clCreateBuffer( _context, CL_MEM_READ_WRITE, sizeof( float  ) * wl_size * v_size               , NULL, NULL );

    _global_work_size = v_size * _mu_size * 2;
    _local_work_size  = _mu_size * 2;

}

ES::Synow::MOATSource::~MOATSource()
{
    delete [] _mu;
    delete [] _dmu;
    delete [] _shift;
    delete [] _start;
    delete [] _in;
    moat_ocl_binary_free( _binary );
    moat_cleanup();
    // TODO deallocate more?
}

void ES::Synow::MOATSource::operator() ( const ES::Synow::Setup& setup )
{

    size_t i;

    // Local caching.  TODO Cast?

    double v_phot  = setup.v_phot;
    double v_outer = setup.v_outer;
    double v_step  = _grid->v[ 1 ] - _grid->v[ 0 ];
    size_t v_size  = _grid->v_size;
    size_t wl_used = _grid->wl_used;

    // Wavelength-independent source-integral metadata.

    i = 0;
    for( size_t iv = 0; iv < v_size; ++ iv )
    {

        float vv      = _grid->v[ iv ];
        float mu_crit = iv > 0 ? sqrt( 1.0 - v_phot * v_phot / vv / vv ) : 0.0;

        float tmp_dmu;
        float mu_init;

        // Rays originating at the photosphere.

        tmp_dmu = ( 1.0 - mu_crit ) / float( _mu_size );
        mu_init = 1.0 - 0.5 * tmp_dmu;

        for( size_t im = 0; im < _mu_size; ++ im )
        {
            _mu   [ i ] = mu_init - im * tmp_dmu;
            _dmu  [ i ] = tmp_dmu;
            _shift[ i ] = 1.0 / ( 1.0 + ( vv * _mu[ i ] - sqrt( vv * vv * ( _mu[ i ] * _mu[ i ] - 1.0 ) + v_phot  * v_phot  ) ) / 299.792 );
            ++ i;
        }

        // Rays originating in the sky.

        tmp_dmu = ( mu_crit + 1.0 ) / float( _mu_size );
        mu_init = mu_crit - 0.5 * tmp_dmu;

        for( size_t im = 0; im < _mu_size; ++ im )
        {
            _mu   [ i ] = mu_init - im * tmp_dmu;
            _dmu  [ i ] = tmp_dmu;
            _shift[ i ] = 1.0 / ( 1.0 + ( vv * _mu[ i ] + sqrt( vv * vv * ( _mu[ i ] * _mu[ i ] - 1.0 ) + v_outer * v_outer ) ) / 299.792 );
            ++ i;
        }

    }

    // Adjust photosphere SED table as needed.

    {
        double blue_wl = _grid->wl[ 0 ];
        for( size_t j = 0; j < v_size * _mu_size; ++ j )
        {
            double shift_wl = _grid->wl[ 0 ] * _shift[ j ];
            if( shift_wl < blue_wl ) blue_wl = shift_wl;
        }
        (*_grid->bb)( blue_wl );
    }

    // Wavelength-dependent source function quantities.

    i = 0;
    for( size_t iw = 0; iw < wl_used; ++ iw )
    {

        size_t j = 0;

        for( size_t iv = 0; iv < v_size; ++ iv )
        {

            // Rays originating at the photosphere.

            for( size_t im = 0; im < _mu_size; ++ im )
            {
                _start[ i ] = 0;
                float start_wl = _grid->wl[ iw ] * _shift[ j ];
                while( _grid->wl[ _start[ i ] ] < start_wl ) ++ _start[ i ];
                _in[ i ] = (*_grid->bb)( _grid->wl[ iw ] * _shift[ i ] ) * pow( _shift[ i ], 3 );

                ++ i;
                ++ j;
            }

            // Rays originating in the sky.

            for( size_t im = 0; im < _mu_size; ++ im )
            {
                _start[ i ] = 0;
                float start_wl = _grid->wl[ iw ] * _shift[ j ];
                while( _grid->wl[ _start[ i ] ] < start_wl ) ++ _start[ i ];
                _in[ i ] = 0.0;

                ++ i;
                ++ j;
            }

        }

    }

    // Enqueue buffer-write events.

    cl_event write_events[ 9 ];

    clEnqueueWriteBuffer( _queue, _dev_v    , CL_TRUE, 0, sizeof( float  ) *           v_size               , (const void*)   _grid->v, 0, NULL, &(write_events[ 0 ]) );
    clEnqueueWriteBuffer( _queue, _dev_mu   , CL_TRUE, 0, sizeof( float  ) *           v_size * _mu_size * 2, (const void*)        _mu, 0, NULL, &(write_events[ 1 ]) );
    clEnqueueWriteBuffer( _queue, _dev_dmu  , CL_TRUE, 0, sizeof( float  ) *           v_size * _mu_size * 2, (const void*)       _dmu, 0, NULL, &(write_events[ 2 ]) );
    clEnqueueWriteBuffer( _queue, _dev_shift, CL_TRUE, 0, sizeof( float  ) *           v_size * _mu_size * 2, (const void*)     _shift, 0, NULL, &(write_events[ 3 ]) );
    clEnqueueWriteBuffer( _queue, _dev_wl   , CL_TRUE, 0, sizeof( float  ) * wl_used                        , (const void*)  _grid->wl, 0, NULL, &(write_events[ 4 ]) );
    clEnqueueWriteBuffer( _queue, _dev_tau  , CL_TRUE, 0, sizeof( float  ) * wl_used * v_size               , (const void*) _grid->tau, 0, NULL, &(write_events[ 5 ]) );
    clEnqueueWriteBuffer( _queue, _dev_start, CL_TRUE, 0, sizeof( size_t ) * wl_used * v_size * _mu_size * 2, (const void*)     _start, 0, NULL, &(write_events[ 6 ]) );
    clEnqueueWriteBuffer( _queue, _dev_in   , CL_TRUE, 0, sizeof( float  ) * wl_used * v_size * _mu_size * 2, (const void*)        _in, 0, NULL, &(write_events[ 7 ]) );
    clEnqueueWriteBuffer( _queue, _dev_src  , CL_TRUE, 0, sizeof( float  ) * wl_used * v_size               , (const void*) _grid->src, 0, NULL, &(write_events[ 8 ]) );

    // Set kernel arguments.

    size_t iw = 0;

    clSetKernelArg( _kernel,  0, sizeof( size_t )                   , (void*) &iw         );
    clSetKernelArg( _kernel,  1, sizeof( size_t )                   , (void*) &v_size     );
    clSetKernelArg( _kernel,  2, sizeof( size_t )                   , (void*) &_mu_size   );
    clSetKernelArg( _kernel,  3, sizeof( size_t )                   , (void*) &wl_used    );
    clSetKernelArg( _kernel,  4, sizeof( cl_mem )                   , (void*) &_dev_v     );
    clSetKernelArg( _kernel,  5, sizeof( float  ) * v_size          ,         NULL        );
    clSetKernelArg( _kernel,  6, sizeof( cl_mem )                   , (void*) &_dev_mu    );
    clSetKernelArg( _kernel,  7, sizeof( cl_mem )                   , (void*) &_dev_dmu   );
    clSetKernelArg( _kernel,  8, sizeof( cl_mem )                   , (void*) &_dev_shift );
    clSetKernelArg( _kernel,  9, sizeof( cl_mem )                   , (void*) &_dev_wl    );
    clSetKernelArg( _kernel, 10, sizeof( float  ) * wl_used         ,         NULL        );
    clSetKernelArg( _kernel, 11, sizeof( cl_mem )                   , (void*) &_dev_tau   );
    clSetKernelArg( _kernel, 12, sizeof( cl_mem )                   , (void*) &_dev_start );
    clSetKernelArg( _kernel, 13, sizeof( cl_mem )                   , (void*) &_dev_in    );
    clSetKernelArg( _kernel, 14, sizeof( float  ) * _local_work_size,         NULL        );
    clSetKernelArg( _kernel, 15, sizeof( cl_mem )                   , (void*) &_dev_src   );

    // Enqueue and execute kernel chain.

    cl_event last_event;
    cl_event exec_event;

    for( size_t iw = 0; iw < wl_used; ++ iw )
    {
        clSetKernelArg( _kernel, 0, sizeof( size_t ), (void*) &iw );
        clEnqueueNDRangeKernel( _queue, _kernel, 1, NULL, &_global_work_size, &_local_work_size, 1, &last_event, &exec_event );
        last_event = exec_event;
    }

    // Read back source function.

    cl_event read_event;
    clEnqueueReadBuffer( _queue, _dev_src, CL_TRUE, 0, sizeof( float ) * wl_used * v_size, _grid->src, 1, &exec_event, &read_event ); // TODO to grid src
    clWaitForEvents( 1, &read_event );

    // Cleanup.

    clFinish( _queue );

}
