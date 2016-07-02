// 
// File    : ES_LineManager.cc
// ---------------------------
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

#include "ES_Exception.hh"
#include "ES_Line.hh"
#include "ES_LineManager.hh"

#ifdef FITSIO_CFITSIO
#include "cfitsio/fitsio.h"
#else
#include "fitsio.h"
#endif

#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>

void ES::LineManager::load( const std::vector< int >& ions, std::vector< ES::Line >& lines )
{
    std::vector< int >::const_iterator ion;
    for( ion = ions.begin(); ion != ions.end(); ++ ion ) load( *ion, lines );
}

void ES::LineManager::load( int const ion, std::vector< ES::Line >& lines )
{

    // Compute path to line file.

    std::string ion_file;
    std::stringstream ss;
    ss << std::setw( 4 ) << std::setfill( '0' ) << std::right << ion;
    ss >> ion_file;
    ion_file = _line_dir + "/line.kurucz." + ion_file + ".fits";

    // Open fits file, if possible.

    fitsfile* fits;
    int status = 0;
    fits_open_file( &fits, ion_file.c_str(), 0, &status );
    if( status != 0 ) throw ES::Exception( "Unable to open line list file: '" + ion_file + "'" );

    // Move forward one HDU.

    int hdutype;
    fits_movrel_hdu( fits, 1, &hdutype, &status );

    // Read number of rows from the header.

    long nrows;
    fits_get_num_rows( fits, &nrows, &status );

    // Allocate line record buffer.

    long long* buffer;
    buffer = new long long [ nrows ];

    // Read line records and close.

    int anynul;
    fits_read_col( fits, TLONGLONG, 1, 1, 1, nrows, 0, buffer, &anynul, &status );
    fits_close_file( fits, &status );

    // Inflate line records into individual lines.

    int    i_wl;
    short  i_el, i_gf;

    double rlog        = log( 1.0 + 1.0 / 2000000.0 );
    double ltth        = 0.001 * log( 10.0 );
    double const_hc_EV = 6.62619e-27 * 2.997924562e+10 / 1.602e-12;

    double wl, gf, el;

    for( int i = 0; i < (int) nrows; ++ i )
    {
        i_wl     = int( buffer[ i ] & 0xFFFFFFFF );
        wl       = 10.0 * exp( i_wl * rlog ); // AA
        if( wl < _min_wl || wl > _max_wl ) continue;
        i_el     = short( ( buffer[ i ] >> 32 ) & 0x0000FFFF );
        i_gf     = short( ( buffer[ i ] >> 48 ) & 0xFFFF );
        gf       = exp( ltth * double( i_gf - 16384 ) );               
        el       = exp( ltth * double( i_el - 16384 ) ) * const_hc_EV; // eV
        lines.push_back( ES::Line( ion, wl, gf, el ) );
    }

    // Buzz off.

    delete [] buffer;

}

void ES::LineManager::drop( const std::vector< int >& ions, std::vector< ES::Line >& lines )
{
    std::vector< int >::const_iterator ion;
    for( ion = ions.begin(); ion != ions.end(); ++ ion ) drop( *ion, lines );
}

void ES::LineManager::drop( int const ion, std::vector< ES::Line >& lines )
{
    ES::Line dummy( ion, 0, 0, 0 );
    std::vector< ES::Line >::iterator middle = 
        std::stable_partition( lines.begin(), lines.end(), std::bind2nd( std::not_equal_to< ES::Line >(), dummy ) );
    lines.erase( middle, lines.end() );
}
