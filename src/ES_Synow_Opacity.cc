// 
// File    : ES_Synow_Opacity.cc
// -----------------------------
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

#include "ES_Synow_Opacity.hh"
#include "ES_Exception.hh"
#include "ES_Synow_Grid.hh"
#include "ES_Synow_Setup.hh"
#include "ES_Line.hh"

#include <cmath>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <iostream>

ES::Synow::Opacity::Opacity( ES::Synow::Grid& grid, const std::string& line_dir, const std::string& ref_file,
        const std::string& form, double const v_ref, double const log_tau_min ) :
    ES::Synow::Operator( grid ),
    ES::LineManager( line_dir, grid.min_wl, grid.max_wl ),
    _ref_file( ref_file ),
    _v_ref( v_ref ),
    _log_tau_min( log_tau_min )
{}

void ES::Synow::Opacity::operator() ( const ES::Synow::Setup& setup )
{

    // Synchronize line list.

    _drop_ions( setup );
    _load_ions( setup );

    // Resolve per-ion excitation temperatures.  Sweep ions, assigning
    // a unique temperature to each --- precedence given to last listed.

    std::map< int, double > temp_map;
    for( size_t i = 0; i < setup.ions.size(); ++ i )
    {
        if( ! setup.active[ i ] ) continue;
        temp_map[ setup.ions[ i ] ] = setup.temp[ i ];
    }

    // Resolve per-ion Sobolev reference opacity profiles.

    std::map< int, std::vector< double > > tau_map;
    for( size_t i = 0; i < setup.ions.size(); ++ i )
    {
        if( ! setup.active[ i ] ) continue;
        tau_map[ setup.ions[ i ] ].resize( _grid->v_size, 0.0 );
    }

    for( size_t i = 0; i < setup.ions.size(); ++ i )
    {
        if( ! setup.active[ i ] ) continue;
        double lin_tau = pow( 10.0, setup.log_tau[ i ] );
        std::map< int, std::vector< double > >::iterator tau = tau_map.find( setup.ions[ i ] );
        for( int iv = 0; iv < _grid->v_size; ++ iv )
        {
            if( _grid->v[ iv ] < setup.v_min[ i ] ) continue;
            if( _grid->v[ iv ] > setup.v_max[ i ] ) break;
            tau->second[ iv ] = lin_tau * exp( ( _v_ref - _grid->v[ iv ] ) / setup.aux[ i ] );
        }
    }

    // Initialize the first bin limits, and step the line 
    // iterator up to the first line in the bin.

    double factor = 1.0 + _grid->bin_width / 299.792;
    double min_wl = _grid->min_wl;
    double max_wl = min_wl * factor;
    int    offset = 0;

    std::vector< ES::Line >::iterator line = _lines.begin();
    while( line != _lines.end() && line->wl < min_wl ) ++ line;

    // Iterate over lines, accumulating Sobolev opacity.  Neglect
    // bins that have no opacity values exceeding the threshold.

    double tau_min = pow( 10.0, _log_tau_min );
    while( line != _lines.end() && max_wl < _grid->max_wl )
    {
        if( line->wl < max_wl )
        {
            ES::Line ref_line = _ref_lines[ line->ion ];
            double str = line->wl * line->gf * exp( 11.604506 * ( ref_line.el - line->el ) / temp_map[ line->ion ] ) / 
                ref_line.wl / ref_line.gf;
            std::map< int, std::vector< double > >::iterator tau = tau_map.find( line->ion );
            for( int iv = 0; iv < _grid->v_size; ++ iv ) _grid->tau[ offset + iv ] += tau->second[ iv ] * str;
            ++ line;
        }
        if( line->wl >= max_wl || line == _lines.end() )
        {
            bool keep = false;
            for( int iv = 0; iv < _grid->v_size; ++ iv ) 
            {
                if( _grid->tau[ offset + iv ] < tau_min ) continue;
                keep = true;
                break;
            }
            if( keep )
            {
                _grid->wl[ _grid->wl_used ] = 0.5 * ( min_wl + max_wl );
                ++ _grid->wl_used;
                offset += _grid->v_size;
            }
            else
            {
                for( int iv = 0; iv < _grid->v_size; ++ iv ) _grid->tau[ offset + iv ] = 0.0;
            }
            min_wl = max_wl;
            max_wl *= factor;
        }
    }

}

void ES::Synow::Opacity::_drop_ions( const ES::Synow::Setup& setup )
{

    // List of ions to drop from line list.  Find ions in reference
    // line list that are not present in the ES::Synow::Setup.  These are
    // the ions to drop.  Return early if there are none to drop.

    std::vector< int > ions;
    for( std::map< int, ES::Line >::iterator ref_line = _ref_lines.begin(); ref_line != _ref_lines.end(); ++ ref_line )
    {
        bool found = false;
        for( size_t i = 0; i < setup.ions.size(); ++ i )
        {
            if( ! setup.active[ i ] ) continue;
            if( setup.ions[ i ] != ref_line->first ) continue;
            found = true;
            break;
        }
        if( found ) continue;
        ions.push_back( ref_line->first );
    }
    if( ions.empty() ) return;

    // Remove ions from reference line list.

    for( size_t i = 0; i < ions.size(); ++ i ) _ref_lines.erase( ions[ i ] );

    // Remove ions from line list.  Erasure uses stable partitioning, so
    // sorting the lines after dropping unwanted ions is not needed.

    drop( ions, _lines );

}

void ES::Synow::Opacity::_load_ions( const ES::Synow::Setup& setup )
{

    // List of ions to add to line list.  Find ions in the ES::Synow::Setup
    // that are not present in the reference line list.  These are 
    // the ions to add. Return early if there are none to add.

    std::vector< int > ions;
    for( size_t i = 0; i < setup.ions.size(); ++ i )
    {
        if( ! setup.active[ i ] ) continue;
        std::map< int, ES::Line >::iterator ref_line = _ref_lines.find( setup.ions[ i ] );
        if( ref_line != _ref_lines.end() ) continue;
        ions.push_back( setup.ions[ i ] );
    }
    if( ions.empty() ) return;

    // Ensure that there are no duplicated ions.

    std::sort( ions.begin(), ions.end() );
    std::vector< int >::iterator duplicates = std::unique( ions.begin(), ions.end() );
    ions.erase( duplicates, ions.end() );

    // Load reference lines for new ions.

    std::ifstream stream;
    stream.open( _ref_file.c_str() );
    if( ! stream.is_open() ) throw ES::Exception( "Unable to open reference line list file: '" + _ref_file + "'" );

    for( size_t i = 0; i < ions.size(); ++ i )
    {
        int ion;
        double wl, gf, el;
        bool found = false;
        while( ! stream.eof() )
        {
            stream >> ion;
            stream >> wl;
            stream >> gf;
            stream >> el;
            if( stream.eof() ) break;
            if( ion != ions[ i ] ) continue;
            found = true;
            break;
        }
        if( ! found )
        {
            std::stringstream ss;
            ss << ions[ i ];
            throw ES::Exception( "Unable to find ion in reference line list file: '" + ss.str() + "'" );
        }
        _ref_lines[ ion ] = ES::Line( ion, wl, gf, el );
        stream.clear();
        stream.seekg( 0, std::ios::beg );
    }
    stream.close();

    // Load lines for new ions and re-sort line list.

    load( ions, _lines );
    std::sort( _lines.begin(), _lines.end() );

}
