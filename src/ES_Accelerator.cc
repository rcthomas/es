// 
// File    : ES_Accelerator.cc
// ---------------------------
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

#include "ES_Accelerator.hh"

#include <cmath>

double ES::Accelerator::operator() ( double const x )
{
   std::map< double, double >::iterator right = _cache.upper_bound( x );
   if( right != _cache.end() )
   {
      std::map< double, double >::iterator left = right;
      -- left;
      if( left != right )
      {
         double a = ( right->first - x ) / ( right->first - left->first );
         double b = 1.0 - a;
         return a * left->second + b * right->second;
      }
      else
      {
         double y = evaluate( x );
         _insert( x, y );
         return y;
      }
   }
   else
   {
      double y = evaluate( x );
      _insert( x, y );
      return y;
   }
}

void ES::Accelerator::_insert( double const x, double const y )
{
   std::pair< std::map< double, double >::iterator, bool > result = _cache.insert( std::pair< double, double >( x, y ) );
   std::map< double, double >::iterator left  = result.first;
   std::map< double, double >::iterator right = result.first;
   -- left;
   ++ right;
   if( left != result.first && left != _cache.end() )
   {
      double xx = 0.5 * ( left->first  + x );
      double yy = 0.5 * ( left->second + y );
      double yf = evaluate( xx );
      if( fabs( ( yy - yf ) / yf ) > _tolerance ) _insert( xx, yf );
   }
   if( right != _cache.end() )
   {
      double xx = 0.5 * ( x + right->first  );
      double yy = 0.5 * ( y + right->second );
      double yf = evaluate( xx );
      if( fabs( ( yy - yf ) / yf ) > _tolerance ) _insert( xx, yf );
   }
}
