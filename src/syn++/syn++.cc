// 
// File    : syn++.cc
// ------------------
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

#include "ES_Synow.hh"

#include <yaml-cpp/yaml.h>

#include <fstream>
#include <getopt.h>
#include <cstdlib>

void operator >> ( const YAML::Node& node, ES::Synow::Setup& setup )
{
    node[ "a0"      ] >> setup.a0;
    node[ "a1"      ] >> setup.a1;
    node[ "a2"      ] >> setup.a2;
    node[ "v_phot"  ] >> setup.v_phot;
    node[ "v_outer" ] >> setup.v_outer;
    node[ "t_phot"  ] >> setup.t_phot;
    for( size_t i = 0; i < node[ "ions"    ].size(); ++ i ) setup.ions.push_back( node[ "ions" ][ i ] );
    for( size_t i = 0; i < node[ "active"  ].size(); ++ i ) setup.active.push_back( node[ "active" ][ i ] );
    for( size_t i = 0; i < node[ "log_tau" ].size(); ++ i ) setup.log_tau.push_back( node[ "log_tau" ][ i ] );
    for( size_t i = 0; i < node[ "v_min"   ].size(); ++ i ) setup.v_min.push_back( node[ "v_min" ][ i ] );
    for( size_t i = 0; i < node[ "v_max"   ].size(); ++ i ) setup.v_max.push_back( node[ "v_max" ][ i ] );
    for( size_t i = 0; i < node[ "aux"     ].size(); ++ i ) setup.aux.push_back( node[ "aux" ][ i ] );
    for( size_t i = 0; i < node[ "temp"    ].size(); ++ i ) setup.temp.push_back( node[ "temp" ][ i ] );
}

void usage( std::ostream& stream )
{
    stream << "usage: syn++ [--verbose] [--gpu] control.yaml" << std::endl;
}

int main( int argc, char* argv[] )
{

    // Command line.

    int verbose = 0;
    int gpu     = 0;

    while( 1 )
    {

        static struct option long_options[] =
        {
            { "verbose" , no_argument, &verbose, 1   },
            { "gpu"     , no_argument,     &gpu, 1   },
            { "help"    , no_argument,        0, 'h' }
        };

        int option_index = 0;
                 
        int c = getopt_long( argc, argv, "h", long_options, &option_index );
        if( c == -1 ) break;

        switch( c )
        {
            case 0 :
                break;
            case 'h' :
                usage( std::cout );
                exit( 0 );
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

    YAML::Node yaml;

    {
        std::ifstream  stream( argv[ optind ++ ] );
        YAML::Parser   parser( stream );
        parser.GetNextDocument( yaml );
        stream.close();
    }

    // Output spectrum.  Here we create from range and step size, but
    // other methods are supported.  See the named constructors in 
    // ES::Spectrum.

    ES::Spectrum output = ES::Spectrum::create_from_range_and_step(
            yaml[ "output" ][ "min_wl"  ],
            yaml[ "output" ][ "max_wl"  ],
            yaml[ "output" ][ "wl_step" ] );

    ES::Spectrum reference = ES::Spectrum::create_from_spectrum( output );

    // Grid object.

    ES::Synow::Grid grid = ES::Synow::Grid::create( 
            yaml[ "output" ][ "min_wl"      ],
            yaml[ "output" ][ "max_wl"      ],
            yaml[ "grid"   ][ "bin_width"   ], 
            yaml[ "grid"   ][ "v_size"      ],
            yaml[ "grid"   ][ "v_outer_max" ] );

    // Opacity operator.

    ES::Synow::Opacity opacity( grid,
            yaml[ "opacity" ][ "line_dir"    ],
            yaml[ "opacity" ][ "ref_file"    ],
            yaml[ "opacity" ][ "form"        ],
            yaml[ "opacity" ][ "v_ref"       ],
            yaml[ "opacity" ][ "log_tau_min" ] );

    // Source operator.

    ES::Synow::Operator* source;
    if( ! gpu )
    {
        source = new ES::Synow::Source( grid,
            yaml[ "source" ][ "mu_size" ] );
    }
    else
    {
        source = new ES::Synow::MOATSource( grid,
            yaml[ "source" ][ "mu_size" ] );
    }

    // Spectrum operator.

    ES::Synow::Spectrum spectrum( grid, output, reference,
            yaml[ "spectrum" ][ "p_size"  ],
            yaml[ "spectrum" ][ "flatten" ] );

    // Attach setups one by one.

    int count = 0;

    const YAML::Node& setups = yaml[ "setups" ];
    for( YAML::Iterator iter = setups.begin(); iter != setups.end(); ++ iter )
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
