// 
// File    : ES_Synow_Opacity.hh
// -----------------------------
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
