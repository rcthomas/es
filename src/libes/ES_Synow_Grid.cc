// 
// File    : ES_Synow_Grid.cc
// --------------------------
//
// ES: Elementary Supernova Spectrum Synthesis, Copyright (c) 2010, The
// Regents of the University of California, through Lawrence Berkeley
// National Laboratory (subject to receipt of any required approvals
// from the U.S. Dept. of Energy). All rights reserved.
//
// If you have questions about your rights to use or distribute this
// software, please contact Berkeley Lab's Technology Transfer
// Department at TTD@lbl.gov.
//
// NOTICE. This software was developed under partial funding from the
// U.S. Department of Energy. As such, the U.S. Government has been
// granted for itself and others acting on its behalf a paid-up,
// nonexclusive, irrevocable, worldwide license in the Software to
// reproduce, prepare derivative works, and perform publicly and display
// publicly. Beginning five (5) years after the date permission to
// assert copyright is obtained from the U.S. Department of Energy, and
// subject to any subsequent five (5) year renewals, the U.S. Government
// is granted for itself and others acting on its behalf a paid-up,
// nonexclusive, irrevocable, worldwide license in the Software to
// reproduce, prepare derivative works, distribute copies to the public,
// perform publicly and display publicly, and to permit others to do so. 
//

#include "ES_Blackbody.hh"
#include "ES_Synow_Grid.hh"
#include "ES_Synow_Setup.hh"
#include "ES_Constants.hh"

#include <cmath>
#include <iostream>

#define B_MARGIN_FACTOR 3
#define R_MARGIN_FACTOR 1
#if __cplusplus <= 199711L
#define nullptr NULL
#endif

////////////////////////////////////////////////////////////////////////////////
//
// _zeroptr
//
// reset all pointers to nullptr and set v_user to false
//
////////////////////////////////////////////////////////////////////////////////
void ES::Synow::Grid::_zeroptr(void)
{
	wl = nullptr;            ///< Wavelengths of opacity/source bin centers in Angstroms.
	v = nullptr;             ///< Velocity grid in kkm/s.
	v_user = false;        ///< Velocity grid was specified by user
	tau = nullptr;           ///< Sobolev opacity table.
	src = nullptr;           ///< Source function table.
	bb = nullptr;            ///< Photosphere blackbody function.
}

////////////////////////////////////////////////////////////////////////////////
//
// void constructor
//
// just clear all pointers
//
////////////////////////////////////////////////////////////////////////////////
ES::Synow::Grid::Grid(void)
{
	_zeroptr();
}

////////////////////////////////////////////////////////////////////////////////
//
// copy constructor
//
// allocate all arrays and members based on RHS data; if velocity data is user,
// copy the pointer, not the data
//
////////////////////////////////////////////////////////////////////////////////

ES::Synow::Grid::Grid( const ES::Synow::Grid & i_cRHO) :
    min_wl( i_cRHO.min_wl ), max_wl( i_cRHO.max_wl ), bin_width( i_cRHO.bin_width ), v_size( i_cRHO.v_size ) 
{
	_zeroptr();
	// determine wl size and allocate wl
    wl_size = int( log( max_wl / min_wl ) / log( 1.0 + bin_width * ES::_inv_c ) + 0.5 );
    wl  = new double[ wl_size ];
	if (wl == nullptr)
		std::cerr << "synow: unable to allocate wl in grid." << std::endl;
	// allocate v if it is not a user profile
	v_user = i_cRHO.v_user;
	if (v_user)
		v = i_cRHO.v;
	else
    	v   = new double[  v_size ];
	if (v == nullptr && !v_user)
		std::cerr << "synow: unable to allocate v in grid." << std::endl;
	if (v == nullptr && v_user)
		std::cerr << "synow: user specified velocity list to grid is nullptr." << std::endl;
	// allocate tau
    tau = new double[ wl_size * v_size ];
	if (tau == nullptr)
		std::cerr << "synow: unable to allocate tau in grid." << std::endl;
	// allocate src
    src = new double[ wl_size * v_size ];
	if (src == nullptr)
		std::cerr << "synow: unable to allocate src in grid." << std::endl;
	// allocate bb
    bb  = new ES::Blackbody();
	if (bb == nullptr)
		std::cerr << "synow: unable to allocate bb in grid." << std::endl;

	// zero data
    _zero();
	// copy data from RHS
	if (wl != nullptr && i_cRHO.wl != nullptr )
		memcpy(wl,i_cRHO.wl,sizeof(double) * wl_size);
	if (tau != nullptr  && i_cRHO.tau != nullptr )
		memcpy(tau,i_cRHO.tau,sizeof(double) * wl_size * v_size);
	if (src != nullptr  && i_cRHO.src != nullptr )
		memcpy(src,i_cRHO.src,sizeof(double) * wl_size * v_size);
	if (v != nullptr  && i_cRHO.v != nullptr && !v_user)
		memcpy(v,i_cRHO.v,sizeof(double) * v_size);
	if (bb != nullptr  && i_cRHO.bb != nullptr )
		*bb = *i_cRHO.bb; // assume that the = operator will correctly copy

}

////////////////////////////////////////////////////////////////////////////////
//
// "normal" constructor
//
// allocate all arrays and members based on call parameters; velocity will be 
// specified by the setup (via reset function below)
//
////////////////////////////////////////////////////////////////////////////////

ES::Synow::Grid::Grid( double const min_wl_, double const max_wl_, double const bin_width_, int const v_size_ ) :
    min_wl( min_wl_ ), max_wl( max_wl_ ), bin_width( bin_width_ ), v_size( v_size_ ) 
{
	_zeroptr();
	// determine wl size and allocate wl
    wl_size = int( log( max_wl / min_wl ) / log( 1.0 + bin_width * ES::_inv_c ) + 0.5 );
    wl  = new double[ wl_size ];
	if (wl == nullptr)
		std::cerr << "synow: unable to allocate wl in grid." << std::endl;
	// allocate v if it is not a user profile
	v_user = false;
   	v   = new double[  v_size ];
	if (v == nullptr)
		std::cerr << "synow: unable to allocate v in grid." << std::endl;
	// allocate tau
    tau = new double[ wl_size * v_size ];
	if (tau == nullptr)
		std::cerr << "synow: unable to allocate tau in grid." << std::endl;
	// allocate src
    src = new double[ wl_size * v_size ];
	if (src == nullptr)
		std::cerr << "synow: unable to allocate src in grid." << std::endl;
	// allocate bb
    bb  = new ES::Blackbody();
	if (bb == nullptr)
		std::cerr << "synow: unable to allocate bb in grid." << std::endl;

    _zero();
}

////////////////////////////////////////////////////////////////////////////////
//
// "user velocity" constructor
//
// similar to "normal" constructor, but uses a user-specified velocity array
// this version has a flag to recompute wavelenght min and max based on 
// blueshift due to velocity range
//
////////////////////////////////////////////////////////////////////////////////

ES::Synow::Grid::Grid( double const min_wl_, double const max_wl_, double const bin_width_, int const v_size_ , double const * velocities_, bool bRecompute_wl_range) :
    min_wl( min_wl_ ), max_wl( max_wl_ ), bin_width( bin_width_ ), v_size( v_size_ ) 
{
	if (bRecompute_wl_range)
	{
		if (v_size > 2) // need at least two velocities!
		{
			if (velocities_[0] < 0.0)
			{
				std::cerr << "synow: minimum velocity for grid must be > 0." << std::endl;
				exit(137);
			}

			double v_step = velocities_[1] - velocities_[0];
			for (unsigned int ii = 1; ii < v_size; ii++)
			{
				if (fabs(velocities_[ii] - velocities_[ii - 1] - v_step) > 1.0e-5)// arbitrary threshold, somewhat near machine precision
				{
					std::cerr << "synow: velocity grid must have uniform step size." << std::endl;
					exit(137);
				}
			}
			if (v_step < 0.0)
			{
				std::cerr << "synow: velocity grid must be monotonically increasing." << std::endl;
				exit(137);
			}
			double v_outer_max = velocities_[v_size - 1];
			if (v_outer_max > ES::_c)
			{
				v_outer_max = ES::_c; // make sure that v_outer < c!
				std::cerr << "synow: WARNING:  v_outer limited to c." << std::endl;
			}
			min_wl = min_wl_ / ( 1.0 + B_MARGIN_FACTOR * v_outer_max * ES::_inv_c );
			max_wl = max_wl_ * ( 1.0 + R_MARGIN_FACTOR * v_outer_max * ES::_inv_c );
		}
		else
		{
			std::cerr << "synow: Not enough velocity bins specified.  There must be at least two bins." << std::endl;
			exit(137);
		}
	}
	_zeroptr();
    wl_size = int( log( max_wl / min_wl ) / log( 1.0 + bin_width * ES::_inv_c ) + 0.5 );
    wl  = new double[ wl_size ];
	if (wl == nullptr)
		std::cerr << "synow: unable to allocate wl in grid." << std::endl;
	// allocate v if it is not a user profile
	v_user = true;
   	v   = velocities_;
	if (v == nullptr)
		std::cerr << "synow: WARNING: user specified velocity array is null." << std::endl;
	// allocate tau
    tau = new double[ wl_size * v_size ];
	if (tau == nullptr)
		std::cerr << "synow: unable to allocate tau in grid." << std::endl;
	// allocate src
    src = new double[ wl_size * v_size ];
	if (src == nullptr)
		std::cerr << "synow: unable to allocate src in grid." << std::endl;
	// allocate bb
    bb  = new ES::Blackbody();
	if (bb == nullptr)
		std::cerr << "synow: unable to allocate bb in grid." << std::endl;

    _zero();
}
////////////////////////////////////////////////////////////////////////////////
//
// destructor
//
// delete all arrays and set pointers to null
//
////////////////////////////////////////////////////////////////////////////////

ES::Synow::Grid::~Grid()
{
	if (wl != nullptr
	    delete [] wl;
	if (v != nullptr && !v_user) // if v_user is set, then is an externally controlled pointer, don't delete
	    delete [] v;
	if (tau != nullptr)
	    delete [] tau;
	if (src != nullptr)
	    delete [] src;
	if (bb != nullptr)
	    delete bb;
	_zeroptr();
}

////////////////////////////////////////////////////////////////////////////////
//
// create(1)
//
// create and return a grid with the desired wavelength and velocity range,
// adjusting wavelength range to account for blueshift at maximum velocity
//
////////////////////////////////////////////////////////////////////////////////

ES::Synow::Grid ES::Synow::Grid::create( double const min_output_wl, double const max_output_wl, double const bin_width, 
        int const v_size, double const v_outer_max )
{
	double v_outer_max_lcl = v_outer_max;
	// make sure the maximum velocity is physical
	if (v_outer_max_lcl > ES::_c)
	{
		v_outer_max_lcl = ES::_c; // make sure that v_outer < c!
		std::cerr << "syn++: WARNING:  v_outer limited to c." << std::endl;
	}
    double min_wl = min_output_wl / ( 1.0 + B_MARGIN_FACTOR * v_outer_max_lcl * ES::_inv_c );
    double max_wl = max_output_wl * ( 1.0 + R_MARGIN_FACTOR * v_outer_max_lcl * ES::_inv_c );
    ES::Synow::Grid grid( min_wl, max_wl, bin_width, v_size );
    return grid;
}

////////////////////////////////////////////////////////////////////////////////
//
// create(2)
//
// create and return a grid with the desired wavelength and velocity range,
// adjusting wavelength range to account for blueshift at maximum velocity
// this version uses a user specified velocity array
//
////////////////////////////////////////////////////////////////////////////////

ES::Synow::Grid ES::Synow::Grid::create( double const &min_output_wl, double const &max_output_wl, double const &bin_width, 
        int v_size, double const * velocities_ )
{
	if (v_size > 2) // need at least two velocities!
	{
		if (velocities_[0] < 0.0)
		{
			std::cerr << "synow: minimum velocity for grid must be > 0." << std::endl;
			exit(137);
		}

		double v_step = velocities_[1] - velocities_[0];
		for (unsigned int ii = 1; ii < v_size; ii++)
		{
			if (fabs(velocities_[ii] - velocities_[ii - 1] - v_step) > 1.0e-5)// arbitrary threshold, somewhat near machine precision
			{
				std::cerr << "synow: velocity grid must have uniform step size." << std::endl;
				exit(137);
			}
		}
		if (v_step < 0.0)
		{
			std::cerr << "synow: velocity grid must be monotonically increasing." << std::endl;
			exit(137);
		}
		double dV_Outer_Max_Lcl = velocities_[v_size - 1];
		if (dV_Outer_Max_Lcl > ES::_c)
		{
			dV_Outer_Max_Lcl = ES::_c; // make sure that v_outer < c!
			std::cerr << "synow: WARNING:  v_outer limited to c." << std::endl;
		}
		double min_wl = min_output_wl / ( 1.0 + B_MARGIN_FACTOR * dV_Outer_Max_Lcl * ES::_inv_c );
		double max_wl = max_output_wl * ( 1.0 + R_MARGIN_FACTOR * dV_Outer_Max_Lcl * ES::_inv_c );
		ES::Synow::Grid grid( min_wl, max_wl, bin_width, v_size, velocities_ );
		return grid;
	}
	else
	{
		std::cerr << "synow: Not enough velocity bins specified.  There must be at least two bins." << std::endl;
		exit(137);
	}
}

////////////////////////////////////////////////////////////////////////////////
//
// reset
//
// reset the grid and generate the velocity array based on the setup parameters
//
////////////////////////////////////////////////////////////////////////////////
void ES::Synow::Grid::reset( ES::Synow::Setup& setup )
{
    _zero();
	if (!v_user) // don't overwrite the user velocity  grid
	{ 
		double v_step = ( setup.v_outer - setup.v_phot ) / double( v_size - 1 );
		for( int i = 0; i < v_size - 1; ++ i ) v [ i ] = setup.v_phot + i * v_step;
		v[ v_size - 1 ] = setup.v_outer;
	}
    bb->temp() = setup.t_phot;
    (*bb)( min_wl );
    (*bb)( max_wl );
}
////////////////////////////////////////////////////////////////////////////////
//
// _zero
//
// set all array data (wl, tau, src, and v (if not user)) to 0
//
////////////////////////////////////////////////////////////////////////////////

void ES::Synow::Grid::_zero()
{
    wl_used = 0;
	if (wl != nullptr)
		memset(wl,0,sizeof(double) * wl_size);
	if (v != nullptr && !v_user )
		memset(v,0,sizeof(double) * v_size);
	if (tau != nullptr)
		memset(tau,0,sizeof(double) * wl_size * v_size);
	if (src != nullptr)
		memset(src,0,sizeof(double) * wl_size * v_size);
}

#if __cplusplus <= 199711L
#undef nullptr
#endif

