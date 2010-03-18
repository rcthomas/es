// 
// File    : ES_Spectrum.cc
// ------------------------
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
#include "ES_Exception.hh"

#include "fitsio.h"

#include <cmath>
#include <limits>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>

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

ES::Spectrum ES::Spectrum::create_from_fits_file( const char* file )
{

    ES::Spectrum spectrum;

    // Open fits file, if possible.

    fitsfile* fits;
    int status = 0;
    fits_open_file( &fits, file, 0, &status );
    if( status != 0 ) throw ES::Exception( "Unable to open spectrum file: '" + std::string( file ) + "'" );

    // Primary HDU parameters.

    int  bitpix = 0;
    int  naxis  = 0;
    long naxes[ 1 ];

    fits_get_img_param( fits, 1, &bitpix, &naxis, naxes, &status );
    if( status != 0 ) throw ES::Exception( "Unable to read primary HDU in spectrum file: '" + std::string( file ) + "'" );

    // Wavelength axis.

    double crval1 = 0.0;
    fits_read_key( fits, TDOUBLE, "CRVAL1", &crval1, NULL, &status );
    if( status != 0 ) throw ES::Exception( "Unable to read CRVAL1 in spectrum file: '" + std::string( file ) + "'" );

    double cdelt1 = 0.0;
    fits_read_key( fits, TDOUBLE, "CDELT1", &cdelt1, NULL, &status );
    if( status != 0 ) throw ES::Exception( "Unable to read CDELT1 in spectrum file: '" + std::string( file ) + "'" );

    // Begin setting up spectrum.

    spectrum.resize( naxes[ 0 ] );
    for( int i = 0; i < spectrum.size(); ++ i ) spectrum.wl( i ) = crval1 + i * cdelt1;

    double* buffer = new double [ spectrum.size() ];

    // Fluxes.

    long      fpixel[ 1 ] = { 1 };
    long long nelements = naxes[ 0 ];

    int anynul = 0;
    fits_read_pix( fits, TDOUBLE, fpixel, nelements, 0, buffer, &anynul, &status );
    if( status != 0 ) throw ES::Exception( "Unable to read fluxes in spectrum file: '" + std::string( file ) + "'" );

    for( int i = 0; i < spectrum.size(); ++ i ) spectrum.flux( i ) = buffer[ i ];

    // Move forward one HDU.

    int hdutype;
    fits_movrel_hdu( fits, 1, &hdutype, &status );
    if( status != 0 ) throw ES::Exception( "Unable to find variance HDU in spectrum file: '" + std::string( file ) + "'" );

    // Flux errors.

    fits_read_pix( fits, TDOUBLE, fpixel, nelements, 0, buffer, &anynul, &status );
    if( status != 0 ) throw ES::Exception( "Unable to read variance in spectrum file: '" + std::string( file ) + "'" );

    for( int i = 0; i < spectrum.size(); ++ i ) spectrum.flux_error( i ) = sqrt( buffer[ i ] );

    // Clean up.

    delete [] buffer;

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

void ES::Spectrum::rescale_median_flux( double const median )
{
    std::vector< double > flux( _flux );
    std::sort( flux.begin(), flux.end() );
    int middle = size() / 2;
    double old_median = size() & 2 == 0 ? 0.5 * ( flux[ middle ] + flux[ middle + 1 ] ) : flux[ middle ];
    double factor = median / old_median;
    for( int i = 0; i < size(); ++ i )
    {
        _flux[ i ] *= factor;
        _flux_error[ i ] *= factor;
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
