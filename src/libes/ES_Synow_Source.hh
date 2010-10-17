// 
// File    : ES_Synow_Source.hh
// ----------------------------
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

#ifndef ES__SYNOW__SOURCE
#define ES__SYNOW__SOURCE

#include "ES_Synow_Operator.hh"

#include <vector>
#include <map>

namespace ES
{

    namespace Synow
    {

        class Grid;

        class Setup;

        /// @class Source
        /// @brief Computes a pure resonance scattering source function.
        ///
        /// This operator computes the pure-resonance scattering line source
        /// functions for opacity bins in the Grid.  As this is the most 
        /// numerically intensive part of the calculation, OpenMP-support is
        /// enabled if it is detected at compile-time.  This calculation is
        /// performed using the familiar ray-tracing method --- not a Monte 
        /// Carlo calculation.  So, while it is faster it is not accurate 
        /// enough to be used for a radiative equilibrium calculation.

        class Source : public ES::Synow::Operator
        {

            public :

                /// Constructor.

                Source( ES::Synow::Grid& grid, int const mu_size );

                /// Destructor.

                ~Source();

                /// Execute a Setup.

                virtual void operator() ( const ES::Synow::Setup& setup );

            private :

                // Note that the full compliment of angles at each point is 
                // 2 * mu_size --- one set is for rays subtending the sky 
                // and the other set is for rays subtending the photosphere.
                // This implementation is actually a good compromise of speed
                // and accuracy, and was first used in the original Synow.
                // The resolution requirements are pretty lax here.

                int      _mu_size;      ///< Number of angles subtending either sky or photosphere.
                double*  _mu;           ///< List of angles as direction-cosines.
                double*  _dmu;          ///< Step in direction-cosine units for integral.
                double*  _shift;        ///< Minimum Doppler first-order Doppler shift along each ray.

        };

    }

}

#endif
