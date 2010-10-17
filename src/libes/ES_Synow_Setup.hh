// 
// File    : ES_Synow_Setup.hh
// ---------------------------
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

                void resize( int const num_ions );

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
