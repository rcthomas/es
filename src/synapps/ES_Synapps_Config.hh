// 
// File    : ES_Synapps_Config.hh
// ------------------------------
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
