// 
// File    : ES_Synapps_Config.cc
// ------------------------------
// Created : Fri Feb 26 10:58:34 2010
// Authors : Rollin C. Thomas (RCT) - rcthomas@lbl.gov
// 

#include "ES_Synapps_Config.hh"

#include "APPSPACK_Float.hpp"
#include "APPSPACK_Vector.hpp"
#include "APPSPACK_Parameter_List.hpp"

#include <yaml-cpp/yaml.h>

#include <set>

ES::Synapps::Config::Config( const YAML::Node& config )
{

   config[ "fit_file" ] >> fit_file;
   std::string cache_file = config[ "cache_file" ];

   params.sublist( "Solver" ).setParameter( "Cache Input File"     , cache_file );
   params.sublist( "Solver" ).setParameter( "Cache Output File"    , cache_file );
   params.sublist( "Solver" ).setParameter( "Precision"            , 2    );
   params.sublist( "Solver" ).setParameter( "Use Random Order"     , true );
   params.sublist( "Solver" ).setParameter( "Use Projected Compass", true );
   params.sublist( "Solver" ).setParameter( "Step Tolerance"       , 0.001 );
   params.sublist( "Solver" ).setParameter( "Contraction Factor"   , 0.9 );
   params.sublist( "Solver" ).setParameter( "Sufficient Decrease Factor"   , 0.1 );

   int num_ions = 0;
   for( int i = 0; i < config[ "active" ].size(); ++ i )
   {
      num_ions += config[ "active" ][ i ] ? 1 : 0;
   }

   APPSPACK::Vector buffer( 6 + 5 * num_ions );

   APPSPACK::Matrix ineq_matrix;
   APPSPACK::Matrix eq_matrix;
   APPSPACK::Vector eq_bound;

   int j;

   // Initial value.

   buffer[ 0 ] = config[ "a0"      ][ "start" ];
   buffer[ 1 ] = config[ "a1"      ][ "start" ];
   buffer[ 2 ] = config[ "a2"      ][ "start" ];
   buffer[ 3 ] = config[ "v_phot"  ][ "start" ];
   buffer[ 4 ] = config[ "v_outer" ][ "start" ];
   buffer[ 5 ] = config[ "t_phot"  ][ "start" ];

   j = 6;
   for( int i = 0; i < config[ "active" ].size(); ++ i )
   {
      if( ! config[ "active" ][ i ] ) continue;
      buffer[ j + 0 * num_ions ] = config[ "log_tau" ][ "start" ][ i ];
      buffer[ j + 1 * num_ions ] = config[ "v_min"   ][ "start" ][ i ];
      buffer[ j + 2 * num_ions ] = config[ "v_max"   ][ "start" ][ i ];
      buffer[ j + 3 * num_ions ] = config[ "aux"     ][ "start" ][ i ];
      buffer[ j + 4 * num_ions ] = config[ "temp"    ][ "start" ][ i ];
      ++ j;
   }

   params.sublist( "Solver" ).setParameter( "Initial X", buffer );

   // Lower boundary.

   buffer[ 0 ] = config[ "a0"      ][ "lower" ];
   buffer[ 1 ] = config[ "a1"      ][ "lower" ];
   buffer[ 2 ] = config[ "a2"      ][ "lower" ];
   buffer[ 3 ] = config[ "v_phot"  ][ "lower" ];
   buffer[ 4 ] = config[ "v_outer" ][ "lower" ];
   buffer[ 5 ] = config[ "t_phot"  ][ "lower" ];

   j = 6;
   for( int i = 0; i < config[ "active" ].size(); ++ i )
   {
      if( ! config[ "active" ][ i ] ) continue;
      buffer[ j + 0 * num_ions ] = config[ "log_tau" ][ "lower" ][ i ];
      buffer[ j + 1 * num_ions ] = config[ "v_min"   ][ "lower" ][ i ];
      buffer[ j + 2 * num_ions ] = config[ "v_max"   ][ "lower" ][ i ];
      buffer[ j + 3 * num_ions ] = config[ "aux"     ][ "lower" ][ i ];
      buffer[ j + 4 * num_ions ] = config[ "temp"    ][ "lower" ][ i ];
      ++ j;
   }

   params.sublist( "Linear" ).setParameter( "Lower", buffer );

   // Upper boundary.

   buffer[ 0 ] = config[ "a0"      ][ "upper" ];
   buffer[ 1 ] = config[ "a1"      ][ "upper" ];
   buffer[ 2 ] = config[ "a2"      ][ "upper" ];
   buffer[ 3 ] = config[ "v_phot"  ][ "upper" ];
   buffer[ 4 ] = config[ "v_outer" ][ "upper" ];
   buffer[ 5 ] = config[ "t_phot"  ][ "upper" ];

   j = 6;
   for( int i = 0; i < config[ "active" ].size(); ++ i )
   {
      if( ! config[ "active" ][ i ] ) continue;
      buffer[ j + 0 * num_ions ] = config[ "log_tau" ][ "upper" ][ i ];
      buffer[ j + 1 * num_ions ] = config[ "v_min"   ][ "upper" ][ i ];
      buffer[ j + 2 * num_ions ] = config[ "v_max"   ][ "upper" ][ i ];
      buffer[ j + 3 * num_ions ] = config[ "aux"     ][ "upper" ][ i ];
      buffer[ j + 4 * num_ions ] = config[ "temp"    ][ "upper" ][ i ];
      ++ j;
   }

   params.sublist( "Linear" ).setParameter( "Upper", buffer );

   // Parameter scalings.

   buffer[ 0 ] = config[ "a0"      ][ "scale" ];
   buffer[ 1 ] = config[ "a1"      ][ "scale" ];
   buffer[ 2 ] = config[ "a2"      ][ "scale" ];
   buffer[ 3 ] = config[ "v_phot"  ][ "scale" ];
   buffer[ 4 ] = config[ "v_outer" ][ "scale" ];
   buffer[ 5 ] = config[ "t_phot"  ][ "scale" ];

   j = 6;
   for( int i = 0; i < config[ "active" ].size(); ++ i )
   {
      if( ! config[ "active" ][ i ] ) continue;
      buffer[ j + 0 * num_ions ] = config[ "log_tau" ][ "scale" ][ i ];
      buffer[ j + 1 * num_ions ] = config[ "v_min"   ][ "scale" ][ i ];
      buffer[ j + 2 * num_ions ] = config[ "v_max"   ][ "scale" ][ i ];
      buffer[ j + 3 * num_ions ] = config[ "aux"     ][ "scale" ][ i ];
      buffer[ j + 4 * num_ions ] = config[ "temp"    ][ "scale" ][ i ];
      ++ j;
   }

   params.sublist( "Linear" ).setParameter( "Scaling", buffer );

   // Inequality bounds constraints: v_phot <= v_outer;

   buffer.zero();
   buffer[ 3 ] = - 1.0;
   buffer[ 4 ] =   1.0;
   ineq_matrix.addRow( buffer );

   // Inequality bounds constraints: v_phot <= v_min.

   j = 6;
   for( int i = 0; i < config[ "active" ].size(); ++ i )
   {
      if( ! config[ "active" ][ i ] ) continue;
      buffer.zero();
      buffer[ 3 ] = - 1.0;
      buffer[ j + 1 * num_ions ] = 1.0;
      ineq_matrix.addRow( buffer );
      ++ j;
   }

   // Inequality bounds constraints: v_min <= v_max.

   j = 6;
   for( int i = 0; i < config[ "active" ].size(); ++ i )
   {
      if( ! config[ "active" ][ i ] ) continue;
      buffer.zero();
      buffer[ j + 1 * num_ions ] = - 1.0;
      buffer[ j + 2 * num_ions ] =   1.0;
      ineq_matrix.addRow( buffer );
      ++ j;
   }

   // Inequality bounds constraints: v_max <= v_outer.

   j = 6;
   for( int i = 0; i < config[ "active" ].size(); ++ i )
   {
      if( ! config[ "active" ][ i ] ) continue;
      buffer.zero();
      buffer[ j + 2 * num_ions ] = - 1.0;
      buffer[ 4 ] = 1.0;
      ineq_matrix.addRow( buffer );
      ++ j;
   }

   // Inequality bounds constraints.

   params.sublist( "Linear" ).setParameter( "Inequality Matrix", ineq_matrix );
   params.sublist( "Linear" ).setParameter( "Inequality Lower" , APPSPACK::Vector( ineq_matrix.getNrows(),             0.0 ) );
   params.sublist( "Linear" ).setParameter( "Inequality Upper" , APPSPACK::Vector( ineq_matrix.getNrows(), APPSPACK::dne() ) );

   // Equality constraints: Detached/attached ions (attached: v_phot == v_min ).

   j = 6;
   for( int i = 0; i < config[ "active" ].size(); ++ i )
   {
      if( ! config[ "active" ][ i ] ) continue;
      if( ! config[ "detach" ][ i ] )
      {
         buffer.zero();
         buffer[ 3 ] = - 1.0;
         buffer[ j + 1 * num_ions ] = 1.0;
         eq_matrix.addRow( buffer );
         eq_bound.push_back( 0.0 );
      }
      ++ j;
   }

   // Equality constraints: Repeated ions must have same temperature.

   std::set< int > done;

   j = 6;
   for( int i = 0; i < config[ "active" ].size(); ++ i )
   {
      if( ! config[ "active" ][ i ] ) continue;
      if( done.find( config[ "ions" ][ i ] ) == done.end() )
      {
         int ion = config[ "ions" ][ i ];
         int jj = 6;
         for( int ii = 0; ii < config[ "active" ].size(); ++ ii )
         {
            if( ! config[ "active" ][ ii ] ) continue;
            if( ii > i && config[ "ions" ][ ii ] == ion )
            {
               buffer.zero();
               buffer[ j  + 4 * num_ions ] = - 1.0;
               buffer[ jj + 4 * num_ions ] =   1.0;
               eq_matrix.addRow( buffer );
               eq_bound.push_back( 0.0 );
            }
            ++ jj;
         }
      }
      done.insert( config[ "ions" ][ i ] );
      ++ j;
   }

   // Equality constraints: Any fixed individual parameters.

   if( config[ "a0" ][ "fixed" ] )
   {
      buffer.zero();
      buffer[ 0 ] = 1.0;
      eq_matrix.addRow( buffer );
      eq_bound.push_back( config[ "a0" ][ "start" ] );
   }

   if( config[ "a1" ][ "fixed" ] )
   {
      buffer.zero();
      buffer[ 1 ] = 1.0;
      eq_matrix.addRow( buffer );
      eq_bound.push_back( config[ "a1" ][ "start" ] );
   }

   if( config[ "a2" ][ "fixed" ] )
   {
      buffer.zero();
      buffer[ 2 ] = 1.0;
      eq_matrix.addRow( buffer );
      eq_bound.push_back( config[ "a2" ][ "start" ] );
   }

   if( config[ "v_phot" ][ "fixed" ] )
   {
      buffer.zero();
      buffer[ 3 ] = 1.0;
      eq_matrix.addRow( buffer );
      eq_bound.push_back( config[ "v_phot" ][ "start" ] );
   }

   if( config[ "v_outer" ][ "fixed" ] )
   {
      buffer.zero();
      buffer[ 4 ] = 1.0;
      eq_matrix.addRow( buffer );
      eq_bound.push_back( config[ "v_outer" ][ "start" ] );
   }

   if( config[ "t_phot" ][ "fixed" ] )
   {
      buffer.zero();
      buffer[ 5 ] = 1.0;
      eq_matrix.addRow( buffer );
      eq_bound.push_back( config[ "t_phot" ][ "start" ] );
   }

   j = 6;
   for( int i = 0; i < config[ "active" ].size(); ++ i )
   {
      if( ! config[ "active" ][ i ] ) continue;
      if( config[ "log_tau" ][ "fixed" ][ i ] )
      {
         buffer.zero();
         buffer[ j + 0 * num_ions ] = 1.0;
         eq_matrix.addRow( buffer );
         eq_bound.push_back( config[ "log_tau" ][ "start" ][ i ] );
      }
      ++ j;
   }

   j = 6;
   for( int i = 0; i < config[ "active" ].size(); ++ i )
   {
      if( ! config[ "active" ][ i ] ) continue;
      if( config[ "v_min" ][ "fixed" ][ i ] )
      {
         buffer.zero();
         buffer[ j + 1 * num_ions ] = 1.0;
         eq_matrix.addRow( buffer );
         eq_bound.push_back( config[ "v_min" ][ "start" ][ i ] );
      }
      ++ j;
   }

   j = 6;
   for( int i = 0; i < config[ "active" ].size(); ++ i )
   {
      if( ! config[ "active" ][ i ] ) continue;
      if( config[ "v_max" ][ "fixed" ][ i ] )
      {
         buffer.zero();
         buffer[ j + 2 * num_ions ] = 1.0;
         eq_matrix.addRow( buffer );
         eq_bound.push_back( config[ "v_max" ][ "start" ][ i ] );
      }
      ++ j;
   }

   j = 6;
   for( int i = 0; i < config[ "active" ].size(); ++ i )
   {
      if( ! config[ "active" ][ i ] ) continue;
      if( config[ "aux" ][ "fixed" ][ i ] )
      {
         buffer.zero();
         buffer[ j + 3 * num_ions ] = 1.0;
         eq_matrix.addRow( buffer );
         eq_bound.push_back( config[ "aux"   ][ "start" ][ i ] );
      }
      ++ j;
   }

   j = 6;
   for( int i = 0; i < config[ "active" ].size(); ++ i )
   {
      if( ! config[ "active" ][ i ] ) continue;
      if( config[ "temp" ][ "fixed" ][ i ] )
      {
         buffer.zero();
         buffer[ j + 4 * num_ions ] = 1.0;
         eq_matrix.addRow( buffer );
         eq_bound.push_back( config[ "temp"  ][ "start" ][ i ] );
      }
      ++ j;
   }
   
   // Equality constraints.

   params.sublist( "Linear" ).setParameter( "Equality Matrix", eq_matrix );
   params.sublist( "Linear" ).setParameter( "Equality Bound" , eq_bound  );

}
