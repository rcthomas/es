// 
// File    : ES_Synow_Setup.hh
// ---------------------------
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

#ifndef ES__SYNOW__SETUP
#define ES__SYNOW__SETUP

#include <vector>
#include <string>
#include <iostream>

namespace ES
{

    namespace Synow
    {

        /// @class Setup
        /// @brief Controls an ES calculation.
        ///
        /// This class carries problem specifications to a Grid, to be
        /// passed in succession to the operators bound to the Grid.  
        /// The end result should be a Spectrum.

        class Setup
        {

            public :         

                /// Resize ion table and associated quantities.

                void resize( size_t const num_ions );

                /// Settings from STL vector.

                bool operator() ( const std::vector< double >& x );

                std::vector< int  > ions;        ///< Ion code table.
                std::vector< bool > active;      ///< Masks ions on or off.

                double a0;                       ///< Spectral warping coefficient.
                double a1;                       ///< Spectral warping coefficient.
                double a2;                       ///< Spectral warping coefficient.
                double v_phot;                   ///< Ejecta velocity at photosphere in kkm/s.
                double v_outer;                  ///< Ejecta velocity at edge of line forming region in kkm/s.
                double t_phot;                   ///< Photosphere blackbody temperature in kK.
                std::vector< double > log_tau;   ///< Opacity parameter (natural logarithm).
                std::vector< double > v_min;     ///< Minimum velocity for opacity parameter in kkm/s.
                std::vector< double > v_max;     ///< Minimum velocity for opacity parameter in kkm/s.
                std::vector< double > aux;       ///< Opacity profile length-scale parameter (kkm/s).
                std::vector< double > temp;      ///< Boltzmann excitation temperature in kK.

        };

    }

}

#endif
