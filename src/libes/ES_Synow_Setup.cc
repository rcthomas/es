//
// File    : ES_Synow_Setup.cc
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

#include "ES_Synow_Setup.hh"

void ES::Synow::Setup::resize( size_t const num_ions )
{
    ions.resize   ( num_ions );
    active.resize ( num_ions );
    log_tau.resize( num_ions );
    v_min.resize  ( num_ions );
    v_max.resize  ( num_ions );
    aux.resize    ( num_ions );
    temp.resize   ( num_ions );
}

bool ES::Synow::Setup::operator() ( const std::vector< double >& x )
{
    a0       = x[ 0 ];
    a1       = x[ 1 ];
    a2       = x[ 2 ];
    v_phot   = x[ 3 ];
    v_outer  = x[ 4 ];
    t_phot   = x[ 5 ];
    for( size_t i = 0; i < ions.size(); ++ i )
    {
        log_tau[ i ] = x[ 6 + 0 * ions.size() + i ];
        v_min  [ i ] = x[ 6 + 1 * ions.size() + i ];
        v_max  [ i ] = x[ 6 + 2 * ions.size() + i ];
        aux    [ i ] = x[ 6 + 3 * ions.size() + i ];
        temp   [ i ] = x[ 6 + 4 * ions.size() + i ];
    }
    return false;
}
