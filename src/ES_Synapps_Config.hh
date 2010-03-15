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
      /// @brief Configuration management for Synapps.
      ///
      /// This class is responsible for managing the configuration of a
      /// Synapps calculation.  This means managing the APPSPACK solver
      /// initialization, as well as the appropriate bounds, linear 
      /// inequality, and linear equality constraints for our ES type
      /// of calculation.  The main interface is through passing a 
      /// YAML file.

      class Config
      {

         public :

            /// Constructor.

            Config( const YAML::Node& yaml );

            std::string fit_file;             ///< Document me.

            APPSPACK::Parameter::List params; ///< APPSPACK parameter list.

      };

   }

}

#endif
