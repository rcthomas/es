// 
// File    : snprep.cc
// -------------------
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

#include "ES_Spectrum.hh"

#include <cstdlib>
#include <iostream>

int main( int argc, char* argv[] )
{
    if( argc != 2 ) exit( 0 );
    ES::Spectrum spectrum = ES::Spectrum::create_from_fits_file( argv[ 1 ] );
    spectrum.rescale_median_flux();
    std::cout << spectrum;
    return 0;
}
