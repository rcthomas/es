// 
// File    : ES_Synapps_Config.hh
// ------------------------------
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

#ifndef ES__SYNAPPS__CONFIG
#define ES__SYNAPPS__CONFIG

#include <appspack/APPSPACK_Parameter_List.hpp>

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
