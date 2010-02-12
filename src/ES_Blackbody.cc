// 
// File    : ES_Blackbody.cc
// -------------------------
// Created : Fri Feb  5 16:34:57 2010
// Authors : Rollin C. Thomas (RCT) - rcthomas@lbl.gov
//  

#include "ES_Blackbody.hh"

#include <cmath>

double ES::Blackbody::evaluate( double const wl ) const
{
   double owl = 1.0 / wl;
   return _temp > 0.0 ? 1.0e20 * owl * owl * owl * owl * owl / ( exp( 143877.505592 * owl / _temp ) - 1.0 ) : 1.0;
}
