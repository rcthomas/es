// 
// File    : ES_Synow_Opacity.cc
// -----------------------------
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

#include "ES_Synow_Opacity.hh"
#include "ES_Exception.hh"
#include "ES_Synow_Grid.hh"
#include "ES_Synow_Setup.hh"
#include "ES_Line.hh"
#include "ES_Constants.hh"

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
{
	_e_form = form_default;
	if (form == "exp" || form == "EXP" || form == "Exp" || form == "exponential" || form == "Exponential" || form == "EXPONENTIAL")
		_e_form = form_exp;
	else if (form == "power" || form == "pow" || form == "Power" || form == "POWER" || form == "POW" || form == 
"Pow")
		_e_form = form_power;
	else if (form == "user_profile" || form == "user" || form == "User" || form == "USER" || form == "User_profile" || form == "User_Profile")
		_e_form = form_user_profile;
}

void ES::Synow::Opacity::operator() ( const ES::Synow::Setup& setup )
{

    // Synchronize line list.

    _drop_ions( setup );
    _load_ions( setup );


    std::map< int, double > temp_map;
    std::map< int, std::vector< double > > tau_map;
    for( size_t i = 0; i < setup.ions.size(); ++ i )
    {
        if( ! setup.active[ i ] ) continue;
    // Resolve per-ion excitation temperatures.  Sweep ions, assigning
    // a unique temperature to each --- precedence given to last listed.
        temp_map[ setup.ions[ i ] ] = setup.temp[ i ];
    // Resolve per-ion Sobolev reference opacity profiles.
        tau_map[ setup.ions[ i ] ].resize( _grid->v_size, 0.0 );
    }

	double tau_lcl;
	double aux,vmin,vmax;
    for( size_t i = 0; i < setup.ions.size(); ++ i )
    {
        if( ! setup.active[ i ] ) continue;
        double lin_tau = pow( 10.0, setup.log_tau[ i ] );
        std::map< int, std::vector< double > >::iterator tau = tau_map.find( setup.ions[ i ] );
		profile_type eFormLcl = form_exp;
		if (!setup.form.empty())
		{
//#pragma omp atomic read
			switch (setup.form[i])
			{
			case ES::Synow::Setup::form_exp:
			default:
				eFormLcl = form_exp;
				break;
			case ES::Synow::Setup::form_power:
				eFormLcl = form_power;
				break;
			case ES::Synow::Setup::form_user_profile:
				eFormLcl = form_user_profile;
				break;
			}
		}
		else
			eFormLcl = _e_form;
		if (eFormLcl == form_exp || eFormLcl == form_power)
		{
			aux = setup.aux[i];
			vmin = setup.v_min[i];
			vmax = setup.v_max[i];
		}
		double taulcl;
//#pragma omp parallel for private(taulcl)
        for( int iv = 0; iv < _grid->v_size; ++ iv )
        {
            if( _grid->v[iv] >= setup.v_phot && (eFormLcl == form_user_profile || (_grid->v[iv] >= vmin && _grid->v[iv] <= vmax)) )
			{
				switch (eFormLcl)
				{
				case form_exp:
				default:
					taulcl = lin_tau * exp( ( _v_ref - _grid->v[iv] ) / aux );
					break;
				case form_power:
			        taulcl = lin_tau * pow( _grid->v[iv] / _v_ref, aux );
					break;
				case form_user_profile:
			        taulcl = lin_tau * setup.user_profile[i][iv];
					break;
				}
				if (setup.additive_opacities)
				{
//#pragma omp atomic
					tau->second[ iv ] += taulcl;
				}
				else
				{
					tau->second[ iv ] = taulcl;
				}
			}
        }
   }

    // Initialize the first bin limits, and step the line 
    // iterator up to the first line in the bin.

    double factor = 1.0 + _grid->bin_width * ES::_inv_c;
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
            for( int iv = 0; iv < _grid->v_size; ++ iv ) 
				if (_grid->v[iv] >= setup.v_phot) 
					_grid->tau[ offset + iv ] += tau->second[ iv ] * str;
            ++ line;
        }

        if( line->wl >= max_wl || line == _lines.end() )
        {
            bool keep = false;
            for( int iv = 0; iv < _grid->v_size && !keep; ++ iv ) 
            {
                keep = ( _grid->tau[ offset + iv ] >= tau_min );
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
        for( size_t i = 0; i < setup.ions.size() && !found; ++ i )
        {
			found = (setup.active[ i ] && setup.ions[ i ] == ref_line->first);
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
