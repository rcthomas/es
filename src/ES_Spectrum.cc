// 
// File    : ES_Spectrum.cc
// ------------------------
// Created : Tue 22 Apr 2008 10:55:02 AM PDT
// Authors : Rollin C. Thomas (ES) - rcthomas@lbl.gov
//  

#include "ES_Spectrum.hh"

#include <limits>
#include <string>
#include <sstream>
#include <fstream>

ES::Spectrum ES::Spectrum::create_from_size( int const size )
{
   ES::Spectrum spectrum;
   spectrum.resize( size );
   return spectrum;
}

ES::Spectrum ES::Spectrum::create_from_range_and_step( double const min_wl, double const max_wl, double const wl_step )
{
   ES::Spectrum spectrum;
   spectrum._wl.push_back( min_wl );
   while( spectrum._wl.back() + wl_step <= max_wl ) spectrum._wl.push_back( spectrum._wl.back() + wl_step );
   spectrum._flux.resize( spectrum._wl.size() );
   spectrum._flux_error.resize( spectrum._wl.size() );
   return spectrum;
}

ES::Spectrum ES::Spectrum::create_from_range_and_size( double const min_wl, double const max_wl, int const size )
{
   ES::Spectrum spectrum;
   double wl_step = ( max_wl - min_wl ) / double( size - 1 );
   spectrum.resize( size );
   for( int i = 0; i < spectrum.size(); ++ i ) spectrum.wl( i ) = min_wl + i * wl_step;
   spectrum.wl( size - 1 ) = max_wl;
   return spectrum;
}

ES::Spectrum ES::Spectrum::create_from_ascii_file( const char* file )
{
   ES::Spectrum spectrum;
   std::ifstream stream;
   stream.open( file );
   stream >> spectrum;
   stream.close();
   return spectrum;
}

ES::Spectrum ES::Spectrum::create_from_spectrum( const ES::Spectrum& original )
{
   ES::Spectrum spectrum;
   spectrum.resize( original.size() );
   for( int i = 0; i < original.size(); ++ i ) spectrum.wl( i ) = original.wl( i );
   return spectrum;
}

void ES::Spectrum::resize( int const size )
{
   clear();
   if( size <= 0 ) return;
   _wl.resize( size, 0.0 );
   _flux.resize( size, 0.0 );
   _flux_error.resize( size, 0.0 );
}

void ES::Spectrum::clear()
{
   _wl.clear();
   _flux.clear();
   _flux_error.clear();
}

void ES::Spectrum::zero_out()
{
   for( int i = 0; i < size(); ++ i )
   {
      _wl[ i ] = 0.0;
      _flux[ i ] = 0.0;
      _flux_error[ i ] = 0.0;
   }
}

double ES::Spectrum::min_flux() const
{
   double min = std::numeric_limits< double >::max();
   for( int i = 0; i < size(); ++ i ) if( _flux[ i ] < min ) min = _flux[ i ];
   return min;
}

double ES::Spectrum::max_flux() const
{
   double max = std::numeric_limits< double >::min();
   for( int i = 0; i < size(); ++ i ) if( _flux[ i ] > max ) max = _flux[ i ];
   return max;
}

namespace ES
{
 
   std::istream& operator >> ( std::istream& stream, ES::Spectrum& spectrum )
   {
      std::vector< double > wl;
      std::vector< double > flux;
      std::vector< double > flux_error;
      double tmp_wl, tmp_flux, tmp_flux_error;
      std::string line;
      while( std::getline( stream, line ) )
      {
         std::string::size_type start = line.find_first_not_of( " " );
         if( line.substr( start, 1 ) == "#" ) continue; 
         std::istringstream parser( line );
         parser >> tmp_wl >> tmp_flux >> tmp_flux_error;
         wl.push_back( tmp_wl );
         flux.push_back( tmp_flux );
         flux_error.push_back( tmp_flux_error );
      }
      spectrum.resize( wl.size() );
      for( int i = 0; i < spectrum.size(); ++ i )
      {
         spectrum.wl( i ) = wl[ i ];
         spectrum.flux( i ) = flux[ i ];
         spectrum.flux_error( i ) = flux_error[ i ];
      }
      return stream;
   }
   
   std::ostream& operator << ( std::ostream& stream, const ES::Spectrum& spectrum )
   {
      for( int i = 0; i < spectrum.size(); ++ i )
      {
         stream << spectrum.wl( i )         << " ";
         stream << spectrum.flux( i )       << " ";
         stream << spectrum.flux_error( i );
         stream << std::endl;
      }
      return stream;
   }

}
