// 
// File    : ES_Synapps_Config.hh
// ------------------------------
// Created : Fri Feb 26 10:46:54 2010
// Authors : Rollin C. Thomas (RCT) - rcthomas@lbl.gov
// 

#ifndef ES__SYNAPPS__CONFIG
#define ES__SYNAPPS__CONFIG

#include "APPSPACK_Parameter_List.hpp"

namespace YAML
{
   class Node;
}

namespace ES
{

   namespace Synapps
   {

      /// @class Config
      /// @brief TBD
      ///
      /// TBD
      /// Thing for turning problem spec in YAML into something APPSPACK wants.
      /// TODO 

      class Config
      {

         public :

            /// Constructor.

            Config( const YAML::Node& yaml );

            std::string fit_file;

            APPSPACK::Parameter::List params; ///< APPSPACK parameter list.

      };

   }

}

#endif
