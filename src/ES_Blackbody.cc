// 
// File    : ES_Blackbody.cc
// -------------------------
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

#include "ES_Blackbody.hh"

#include <cmath>

double ES::Blackbody::wl_peak() const
{
    return 50994.364 / _temp;
}

double ES::Blackbody::evaluate( double const wl ) const
{
    double owl = 50994.364 / wl / _temp;
    return owl * owl * owl * ( exp( 2.82143937212 ) - 1.0 ) / ( exp( 2.82143937212 * owl ) - 1.0 );
}
