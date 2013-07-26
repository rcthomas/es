//
// File    : syn++.cc
// ------------------
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

#include "ES_Synow.hh"

#include <yaml-cpp/yaml.h>

#include <fstream>
#include <getopt.h>
#include <cstdlib>
#include <sstream>

void operator >> ( const YAML::Node& node, ES::Synow::Setup& setup )
{
    setup.a0      = node[ "a0"      ].as<double>();
    setup.a1      = node[ "a1"      ].as<double>();
    setup.a2      = node[ "a2"      ].as<double>();
    setup.v_phot  = node[ "v_phot"  ].as<double>();
    setup.v_outer = node[ "v_outer" ].as<double>();
    setup.t_phot  = node[ "t_phot"  ].as<double>();
    for( size_t i = 0; i < node[ "ions"    ].size(); ++ i ) setup.ions.push_back(    node[ "ions"    ][ i ].as<int>()    );
    for( size_t i = 0; i < node[ "active"  ].size(); ++ i ) setup.active.push_back(  node[ "active"  ][ i ].as<bool>()   );
    for( size_t i = 0; i < node[ "log_tau" ].size(); ++ i ) setup.log_tau.push_back( node[ "log_tau" ][ i ].as<double>() );
    for( size_t i = 0; i < node[ "v_min"   ].size(); ++ i ) setup.v_min.push_back(   node[ "v_min"   ][ i ].as<double>() );
    for( size_t i = 0; i < node[ "v_max"   ].size(); ++ i ) setup.v_max.push_back(   node[ "v_max"   ][ i ].as<double>() );
    for( size_t i = 0; i < node[ "aux"     ].size(); ++ i ) setup.aux.push_back(     node[ "aux"     ][ i ].as<double>() );
    for( size_t i = 0; i < node[ "temp"    ].size(); ++ i ) setup.temp.push_back(    node[ "temp"    ][ i ].as<double>() );
}

void usage( std::ostream& stream )
{
    stream << "usage: syn++ [--verbose] control.yaml" << std::endl;
}

int main( int argc, char* argv[] )
{

    // Command line.

    int         verbose = 0;
    std::string target_file;

    while( 1 )
    {

        static struct option long_options[] =
        {
            { "verbose" ,       no_argument, &verbose, 1   },
            { "help"    ,       no_argument,        0, 'h' },
            { "wl-from" , required_argument,        0, 'w' }
        };

        int option_index = 0;
        std::stringstream ss;

        int c = getopt_long( argc, argv, "h", long_options, &option_index );
        if( c == -1 ) break;

        switch( c )
        {
            case 0 :
                break;
            case 'h' :
                usage( std::cout );
                exit( 0 );
            case 'w' :
                ss << optarg;
                ss >> target_file;
                ss.clear();
                break;
            case '?' :
                usage( std::cerr );
                exit( 137 );
            default :
                usage( std::cerr );
                exit( 137 );
        }

    }

    if( ! ( optind < argc ) )
    {
        std::cerr << "syn++: missing control file" << std::endl;
        usage( std::cerr );
        exit( 137 );
    }

    // Configuration in this application comes from a YAML file.

    YAML::Node yaml = YAML::LoadFile( argv[ optind ++ ] );

    // Output spectrum.  Here we create from range and step size, but
    // other methods are supported.  See the named constructors in
    // ES::Spectrum.

    ES::Spectrum output = ES::Spectrum::create_from_range_and_step(
            yaml[ "output" ][ "min_wl"  ].as<double>(),
            yaml[ "output" ][ "max_wl"  ].as<double>(),
            yaml[ "output" ][ "wl_step" ].as<double>() );

    if( ! target_file.empty() ) output = ES::Spectrum::create_from_ascii_file( target_file.c_str() );

    ES::Spectrum reference = ES::Spectrum::create_from_spectrum( output );

    // Grid object.

    ES::Synow::Grid grid = ES::Synow::Grid::create(
            yaml[ "output" ][ "min_wl"      ].as<double>(),
            yaml[ "output" ][ "max_wl"      ].as<double>(),
            yaml[ "grid"   ][ "bin_width"   ].as<double>(),
            yaml[ "grid"   ][ "v_size"      ].as<double>(),
            yaml[ "grid"   ][ "v_outer_max" ].as<double>() );

    // Opacity operator.

    ES::Synow::Opacity opacity( grid,
            yaml[ "opacity" ][ "line_dir"    ].as<std::string>(),
            yaml[ "opacity" ][ "ref_file"    ].as<std::string>(),
            yaml[ "opacity" ][ "form"        ].as<std::string>(),
            yaml[ "opacity" ][ "v_ref"       ].as<double>(),
            yaml[ "opacity" ][ "log_tau_min" ].as<double>() );

    // Source operator.

    ES::Synow::Source source( grid,
            yaml[ "source" ][ "mu_size" ].as<int>() );

    // Spectrum operator.

    ES::Synow::Spectrum spectrum( grid, output, reference,
            yaml[ "spectrum" ][ "p_size"  ].as<int>(),
            yaml[ "spectrum" ][ "flatten" ].as<bool>() );

    // Attach setups one by one.

    int count = 0;

    const YAML::Node& setups = yaml[ "setups" ];
    for( YAML::const_iterator iter = setups.begin(); iter != setups.end(); ++ iter )
    {
        ++ count;
        if( verbose ) std::cerr << "computing spectrum " << count << " of " << setups.size() << std::endl;
        ES::Synow::Setup setup;
        *iter >> setup;
        grid( setup );
        std::cout << output << std::endl;
    }

    return 0;
}
