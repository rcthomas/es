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

    // Prepare a GPU device.

    _dev = moat_ocl_getgpu( 0 );
    //context
    //queue
    //other crap

}

ES::Synow::MOATSource::~MOATSource()
{
    // What's the MOAT cleanup for _dev?
}


void ES::Synow::MOATSource::operator() ( const ES::Synow::Setup& setup )
{
    // ...
}
