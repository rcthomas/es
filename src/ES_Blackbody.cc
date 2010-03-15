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

double ES::Blackbody::evaluate( double const wl ) const
{
   double owl = 1.0 / wl;
   return _temp > 0.0 ? 1.0e20 * owl * owl * owl * owl * owl / ( exp( 143877.505592 * owl / _temp ) - 1.0 ) : 1.0;
}
