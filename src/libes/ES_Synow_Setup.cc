// 
// File    : ES_Synow_Setup.cc
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

#include "ES_Synow_Setup.hh"
ES::Synow::Setup::Setup(void)
{
	a0 = 1.0;                       ///< Spectral warping coefficient.
	a1 = 0.0;                       ///< Spectral warping coefficient.
	a2 = 0.0;                       ///< Spectral warping coefficient.
	v_phot = 0.0;                   ///< Ejecta velocity at photosphere in kkm/s.
	v_outer = 0.0;                  ///< Ejecta velocity at edge of line forming region in kkm/s.
	t_phot = 0.0;                   ///< Photosphere blackbody temperature in kK.
	additive_opacities = false;      ///< opacities for ions of the same type are additive. If this flag is not set the opacity of the last instance is used above the v_min specified for the last instance
}
ES::Synow::Setup::Setup(unsigned int num_ions)
{
	a0 = 1.0;                       ///< Spectral warping coefficient.
	a1 = 0.0;                       ///< Spectral warping coefficient.
	a2 = 0.0;                       ///< Spectral warping coefficient.
	v_phot = 0.0;                   ///< Ejecta velocity at photosphere in kkm/s.
	v_outer = 0.0;                  ///< Ejecta velocity at edge of line forming region in kkm/s.
	t_phot = 0.0;                   ///< Photosphere blackbody temperature in kK.
	additive_opacities = false;      ///< opacities for ions of the same type are additive. If this flag is not set the opacity of the last instance is used above the v_min specified for the last instance
	resize(num_ions);
}

void ES::Synow::Setup::resize( int const num_ions )
{
    ions.resize   ( num_ions );
    active.resize ( num_ions );
    log_tau.resize( num_ions );
    v_min.resize  ( num_ions );
    v_max.resize  ( num_ions );
    aux.resize    ( num_ions );
    temp.resize   ( num_ions );
    form.resize   ( num_ions );
	user_profile.resize (num_ions);
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
