// 
// File    : snprep.cc
// -------------------
// Created : Sun Feb 28 21:32:51 2010
// Authors : Rollin C. Thomas (RCT) - rcthomas@lbl.gov
// 

#include "ES_Spectrum.hh"

#include <iostream>

int main( int argc, char* argv[] )
{
   if( argc != 2 ) exit( 0 );
   ES::Spectrum spectrum = ES::Spectrum::create_from_fits_file( argv[ 1 ] );
   spectrum.rescale_median_flux();
   std::cout << spectrum;
   return 0;
}
