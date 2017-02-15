// 
// File    : ES_Spectrum.cc
// ------------------------
//
// ES: Elementary Supernova Spectrum Synthesis, Copyright (c) 2010, The
// Regents of the University of California, through Lawrence Berkeley
// National Laboratory (subject to receipt of any required approvals
// from the U.S. Dept. of Energy). All rights reserved.
//
// If you have questions about your rights to use or distribute this
// software, please contact Berkeley Lab's Technology Transfer
// Department at TTD@lbl.gov.
//
// NOTICE. This software was developed under partial funding from the
// U.S. Department of Energy. As such, the U.S. Government has been
// granted for itself and others acting on its behalf a paid-up,
// nonexclusive, irrevocable, worldwide license in the Software to
// reproduce, prepare derivative works, and perform publicly and display
// publicly. Beginning five (5) years after the date permission to
// assert copyright is obtained from the U.S. Department of Energy, and
// subject to any subsequent five (5) year renewals, the U.S. Government
// is granted for itself and others acting on its behalf a paid-up,
// nonexclusive, irrevocable, worldwide license in the Software to
// reproduce, prepare derivative works, distribute copies to the public,
// perform publicly and display publicly, and to permit others to do so. 
//

#include "ES_Spectrum.hh"
#include "ES_Exception.hh"
#ifdef FITSIO_CFITSIO
#include "cfitsio/fitsio.h"
#else
#include "fitsio.h"
#endif

#include <cmath>
#include <limits>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>

ES::Spectrum ES::Spectrum::create_from_size( size_t const size )
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

ES::Spectrum ES::Spectrum::create_from_range_and_size( double const min_wl, double const max_wl, size_t const size )
{
    ES::Spectrum spectrum;
    double wl_step = ( max_wl - min_wl ) / double( size - 1 );
    spectrum.resize( size );
    for( size_t i = 0; i < spectrum.size(); ++ i ) spectrum.wl( i ) = min_wl + i * wl_step;
    spectrum.wl( size - 1 ) = max_wl;
    return spectrum;
}

ES::Spectrum ES::Spectrum::create_from_ascii_file( const char* file )
{
    ES::Spectrum spectrum;
    std::ifstream stream;
    stream.open( file );
    if( ! stream.is_open() ) throw ES::Exception( "Unable to open spectrum file: '" + std::string( file ) + "'" );
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
    for( size_t i = 0; i < spectrum.size(); ++ i ) spectrum.wl( i ) = crval1 + i * cdelt1;

    double* buffer = new double [ spectrum.size() ];

    // Fluxes.

    long      fpixel[ 1 ] = { 1 };
    long long nelements = naxes[ 0 ];

    int anynul = 0;
    fits_read_pix( fits, TDOUBLE, fpixel, nelements, 0, buffer, &anynul, &status );
    if( status != 0 ) throw ES::Exception( "Unable to read fluxes in spectrum file: '" + std::string( file ) + "'" );

    for( size_t i = 0; i < spectrum.size(); ++ i ) spectrum.flux( i ) = buffer[ i ];

    // Move forward one HDU.

    int hdutype;
    fits_movrel_hdu( fits, 1, &hdutype, &status );
    if( status != 0 ) throw ES::Exception( "Unable to find variance HDU in spectrum file: '" + std::string( file ) + "'" );

    // Flux errors.

    fits_read_pix( fits, TDOUBLE, fpixel, nelements, 0, buffer, &anynul, &status );
    if( status != 0 ) throw ES::Exception( "Unable to read variance in spectrum file: '" + std::string( file ) + "'" );

    for( size_t i = 0; i < spectrum.size(); ++ i ) spectrum.flux_error( i ) = sqrt( buffer[ i ] );

    // Clean up.

    delete [] buffer;

    return spectrum;
}

ES::Spectrum ES::Spectrum::create_from_spectrum( const ES::Spectrum& original )
{
    ES::Spectrum spectrum;
    spectrum.resize( original.size() );
    for( size_t i = 0; i < original.size(); ++ i ) spectrum.wl( i ) = original.wl( i );
    return spectrum;
}

ES::Spectrum ES::Spectrum::create_from_vector( const std::vector<std::tuple<double, double, double> > i_vtdData )
{
    ES::Spectrum spectrum;
    spectrum.resize( i_vtdData.size() );
    for( size_t i = 0; i < i_vtdData.size(); ++ i ) spectrum.wl( i ) = std::get<0>(i_vtdData[i]);
    return spectrum;
}
ES::Spectrum ES::Spectrum::create_from_wl_vector( const std::vector<double > i_vtdData )
{
    ES::Spectrum spectrum;
    spectrum.resize( i_vtdData.size() );
    for( size_t i = 0; i < i_vtdData.size(); ++ i ) spectrum.wl( i ) = i_vtdData[i];
    return spectrum;
}
ES::Spectrum ES::Spectrum::create_from_array( const double * i_lpdData, size_t i_nNum_Points )
{
    ES::Spectrum spectrum;
    spectrum.resize( i_nNum_Points );
    for( size_t i = 0; i < i_nNum_Points; ++ i ) spectrum.wl( i ) = i_lpdData[i];
    return spectrum;
}

ES::Spectrum ES::Spectrum::create_copy_from_vector( const std::vector<std::tuple<double, double, double> > i_vtdData )
{
    ES::Spectrum spectrum;
    spectrum.resize( i_vtdData.size() );
    for( size_t i = 0; i < i_vtdData.size(); ++ i )
	{
		spectrum.wl( i ) = std::get<0>(i_vtdData[i]);
		spectrum.flux( i ) = std::get<1>(i_vtdData[i]);
		spectrum.flux_error( i ) = std::get<2>(i_vtdData[i]);
	}
    return spectrum;
}


void ES::Spectrum::resize( size_t const size )
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
    for( size_t i = 0; i < size(); ++ i )
    {
        _wl[ i ] = 0.0;
        _flux[ i ] = 0.0;
        _flux_error[ i ] = 0.0;
    }
}
void ES::Spectrum::zero_flux()
{
    for( size_t i = 0; i < size(); ++ i )
    {
        _flux[ i ] = 0.0;
        _flux_error[ i ] = 0.0;
    }
}

void ES::Spectrum::rescale_median_flux( double const median )
{
    std::vector< double > flux( _flux );
    std::sort( flux.begin(), flux.end() );
    int middle = size() / 2;
    double old_median = size() % 2 == 0 ? 0.5 * ( flux[ middle ] + flux[ middle + 1 ] ) : flux[ middle ];
    double factor = median / old_median;
    for( size_t i = 0; i < size(); ++ i )
    {
        _flux[ i ] *= factor;
        _flux_error[ i ] *= factor;
    }
}

double ES::Spectrum::min_flux() const
{
    double min = std::numeric_limits< double >::max();
    for( size_t i = 0; i < size(); ++ i ) if( _flux[ i ] < min ) min = _flux[ i ];
    return min;
}

double ES::Spectrum::max_flux() const
{
    double max = std::numeric_limits< double >::min();
    for( size_t i = 0; i < size(); ++ i ) if( _flux[ i ] > max ) max = _flux[ i ];
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
        for( size_t i = 0; i < spectrum.size(); ++ i )
        {
            spectrum.wl( i ) = wl[ i ];
            spectrum.flux( i ) = flux[ i ];
            spectrum.flux_error( i ) = flux_error[ i ];
        }
        return stream;
    }

    std::ostream& operator << ( std::ostream& stream, const ES::Spectrum& spectrum )
    {
        for( size_t i = 0; i < spectrum.size(); ++ i )
        {
            stream << spectrum.wl( i )         << " ";
            stream << spectrum.flux( i )       << " ";
            stream << spectrum.flux_error( i );
            stream << std::endl;
        }
        return stream;
    }

}


