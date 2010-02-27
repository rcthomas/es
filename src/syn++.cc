// 
// File    : syn++.cc
// ------------------
// Created : Sun Jan 24 13:42:14 2010
// Authors : Rollin C. Thomas (RCT) - rcthomas@lbl.gov
//  

#include "ES_Synow.hh"

#include <yaml-cpp/yaml.h>

#include <fstream>

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

int main( int argc, char* argv[] )
{

   // Configuration in this application comes from a YAML file.

   YAML::Node yaml;

   {
      std::ifstream  stream( argv[ 1 ] );
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

   ES::Synow::Source source( grid,
         yaml[ "source" ][ "mu_size" ] );

   // Spectrum operator.

   ES::Synow::Spectrum spectrum( grid, output,
         yaml[ "spectrum" ][ "p_size" ] );

   // Attach setups one by one.

   const YAML::Node& setups = yaml[ "setups" ];
   for( YAML::Iterator iter = setups.begin(); iter != setups.end(); ++ iter )
   {
      ES::Synow::Setup setup;
      *iter >> setup;
      grid( setup );
      std::cout << output << std::endl;
   }

   return 0;
}
