// 
// File    : ES_Synow_Opacity.hh
// -----------------------------
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

#ifndef ES__SYNOW__OPACITY
#define ES__SYNOW__OPACITY

#include "ES_Synow_Operator.hh"
#include "ES_LineManager.hh"

#include <vector>
#include <map>

namespace ES
{

    namespace Synow
    {

        class Grid;

        class Setup;

        /// @class Opacity
        /// @brief Synow Sobolev opacity operator.
        ///
        /// Manages a list of atomic lines and uses the same reference line
        /// scheme as Synow for parameterizing optical depth profiles.  That
        /// is, 1D spatial functions are use to describe the dependence of
        /// Sobolev optical depth in a reference line; opacity in other lines
        /// of the same ion is scaled relative to that through a temperature,
        /// akin to thermal equilibrium.

        class Opacity : public ES::Synow::Operator, public ES::LineManager
        {

            public :

                /// Constructor.

                Opacity( ES::Synow::Grid& grid, const std::string& line_dir, const std::string& ref_file,
                        const std::string& form, double const v_ref, double const log_tau_min );

                /// Execute a Setup.

                virtual void operator() ( const ES::Synow::Setup& setup );

            private :

                std::string                _ref_file;     ///< Path to reference line list file.
                std::string                _form;         ///< Functional form of reference line opacity profile.
                double                     _v_ref;        ///< Reference velocity in kkm/s for scaling reference line opacity profiles.
                double                     _log_tau_min;  ///< Minimum Sobolev opacity to include a bin.
                std::map< int, ES::Line >  _ref_lines;    ///< Reference lines.
                std::vector< ES::Line >    _lines;        ///< List of loaded lines.

                /// Drop ions from the line list not needed by the Setup.

                void _drop_ions( const ES::Synow::Setup& setup );

                /// Load ions required by the Setup.

                void _load_ions( const ES::Synow::Setup& setup );


        };

    }

}

#endif
