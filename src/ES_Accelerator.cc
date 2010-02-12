// 
// File    : ES_Accelerator.cc
// ---------------------------
// Created : Thu Feb 11 11:43:46 2010
// Authors : Rollin C. Thomas (RCT) - rcthomas@lbl.gov
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
