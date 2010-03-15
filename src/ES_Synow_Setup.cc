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

void ES::Synow::Setup::resize( int const num_ions )
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

// bool ES::Setup::operator() ( lua::state& lua )
// {
//    int value = lua.next();
//    if( ! value ) return value;
//    ions.clear();
//    active.clear();
//    log_tau.clear();
//    v_min.clear();
//    v_max.clear();
//    aux.clear();
//    temp.clear();
//    lua.getfield( "a0"      ).to( a0      ).pop();
//    lua.getfield( "a1"      ).to( a1      ).pop();
//    lua.getfield( "a2"      ).to( a2      ).pop();
//    lua.getfield( "v_phot"  ).to( v_phot  ).pop();
//    lua.getfield( "v_outer" ).to( v_outer ).pop();
//    lua.getfield( "t_phot"  ).to( t_phot  ).pop();
//    lua.getfield( "ions"    ).to( ions    ).pop();
//    lua.getfield( "active"  ).to( active  ).pop();
//    lua.getfield( "log_tau" ).to( log_tau ).pop();
//    lua.getfield( "v_min"   ).to( v_min   ).pop();
//    lua.getfield( "v_max"   ).to( v_max   ).pop();
//    lua.getfield( "aux"     ).to( aux     ).pop();
//    lua.getfield( "temp"    ).to( temp    ).pop();
//    lua.pop();
//    return value;
// }

// std::ostream& ES::Setup::lua_stream( std::ostream& stream ) const
// {
//    std::string pre = "   ";
//    stream << pre << "{" << std::endl;
//    stream << pre << pre << "a0      = " << a0      << "," << std::endl;
//    stream << pre << pre << "a1      = " << a1      << "," << std::endl;
//    stream << pre << pre << "a2      = " << a2      << "," << std::endl;
//    stream << pre << pre << "v_phot  = " << v_phot  << "," << std::endl;
//    stream << pre << pre << "v_outer = " << v_outer << "," << std::endl;
//    stream << pre << pre << "t_phot  = " << t_phot  << "," << std::endl;
//    stream << std::endl;
//    stream << pre << pre << "ions    = { ";
//    for( int i = 0; i < ions.size(); ++ i ) stream << std::setw( 5 ) << ions[ i ] << ", ";
//    stream << "}, " << std::endl;
//    stream << pre << pre << "active  = { ";
//    for( int i = 0; i < ions.size(); ++ i ) stream << std::setw( 5 ) << ( active[ i ] ? "true" : "false" ) << ", ";
//    stream << "}, " << std::endl;
//    stream << pre << pre << "log_tau = { ";
//    for( int i = 0; i < ions.size(); ++ i ) stream << std::setw( 5 ) << log_tau[ i ] << ", ";
//    stream << "}, " << std::endl;
//    stream << pre << pre << "v_min   = { ";
//    for( int i = 0; i < ions.size(); ++ i ) stream << std::setw( 5 ) << v_min[ i ] << ", ";
//    stream << "}, " << std::endl;
//    stream << pre << pre << "v_max   = { ";
//    for( int i = 0; i < ions.size(); ++ i ) stream << std::setw( 5 ) << v_max[ i ] << ", ";
//    stream << "}, " << std::endl;
//    stream << pre << pre << "aux     = { ";
//    for( int i = 0; i < ions.size(); ++ i ) stream << std::setw( 5 ) << aux[ i ] << ", ";
//    stream << "}, " << std::endl;
//    stream << pre << pre << "temp    = { ";
//    for( int i = 0; i < ions.size(); ++ i ) stream << std::setw( 5 ) << temp[ i ] << ", ";
//    stream << "}, " << std::endl;
//    stream << pre << "}," << std::endl;
//    return stream;
// }
