// 
// File    : ES_Synow_MOATSource.hh
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

#ifndef ES__SYNOW__MOATSOURCE
#define ES__SYNOW__MOATSOURCE

#include "ES_Synow_Operator.hh"

#include <moat.h>
#include <moat_ocl.h>

#include <vector>
#include <map>

namespace ES
{

    namespace Synow
    {

        class Grid;

        class Setup;

        /// @class MOATSource
        /// @brief Computes a pure resonance scattering source function using GPU hardware.
        ///
        /// This operator computes the pure-resonance scattering line source
        /// functions for opacity bins in the Grid.  This is typically the 
        /// most numerically intensive part of the whole calculation, so we
        /// take advantage of GPU hardware via OpenCL in this implementation.
        /// This module will be compiled if GPU support is detected at
        /// compile-time.  This calculation is performed using the familiar
        /// ray-tracing method --- not a Monte Carlo calculation.  So, while
        /// it is faster it is not accurate enough to be used for a radiative
        /// equilibrium calculation.

        class MOATSource : public ES::Synow::Operator
        {

            public :

                /// Constructor.

                MOATSource( ES::Synow::Grid& grid, size_t const mu_size );

                /// Destructor.

                ~MOATSource();

                /// Execute a Setup.

                virtual void operator() ( const ES::Synow::Setup& setup );

            private :

                moat_ocl_binary*    _binary;    ///< MOAT OpenCL binary program.
                moat_ocl_dev*       _dev;       ///< MOAT OpenCL device.
                cl_context          _context;   ///< OpenCL context from MOAT.
                cl_command_queue    _queue;     ///< OpenCL command queue from MOAT.
                cl_kernel           _kernel;    ///< OpenCL kernel.
                cl_mem              _dev_v;     ///< OpenCL buffer: velocity grid.
                cl_mem              _dev_mu;    ///< OpenCL buffer: angle grid.
                cl_mem              _dev_dmu;   ///< OpenCL buffer: angle step grid.
                cl_mem              _dev_shift; ///< OpenCL buffer: initial Doppler shifts.
                cl_mem              _dev_wl;    ///< OpenCL buffer: wavelength list.
                cl_mem              _dev_tau;   ///< OpenCL buffer: opacity table.
                cl_mem              _dev_start; ///< OpenCL buffer: integration start bin.
                cl_mem              _dev_in;    ///< OpenCL buffer: initial intensity.
                cl_mem              _dev_src;   ///< OpenCL buffer: source function.

                size_t              _global_work_size;  ///< OpenCL global workgroup size.
                size_t              _local_work_size;   ///< OpenCL local workgroup size.

                // Note that the full compliment of angles at each point is 
                // 2 * mu_size --- one set is for rays subtending the sky 
                // and the other set is for rays subtending the photosphere.
                // This implementation is actually a good compromise of speed
                // and accuracy, and was first used in the original Synow.
                // The resolution requirements are pretty lax here.
                //
                // Note: User should set mu_size to equal a half-warp, so the
                // workgroup is equal to a full warp.

                size_t   _mu_size;      ///< Number of angles subtending either sky or photosphere.
                double*  _mu;           ///< List of angles as direction-cosines.
                double*  _dmu;          ///< Step in direction-cosine units for integral.
                double*  _shift;        ///< Minimum Doppler first-order Doppler shift along each ray.
                size_t*  _start;        ///< Wavelength bin starting points for integration.
                double*  _in;           ///< Initial intensity buffer for integration.

        };

    }

}

#endif
