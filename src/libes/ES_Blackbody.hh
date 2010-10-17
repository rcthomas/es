// 
// File    : ES_Blackbody.hh
// -------------------------
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

#ifndef ES__BLACKBODY
#define ES__BLACKBODY

#include "ES_Accelerator.hh"

#include <map>

namespace ES
{

    /// @class Blackbody
    /// @brief Blackbody spectral energy distribution.
    ///
    /// Normalized to an intensity of approximately 1 at peak, in F-nu units.

    class Blackbody : public ES::Accelerator
    {

        public :

            /// Constructor.

            Blackbody( double const tolerance = 0.001 ) :
                ES::Accelerator( tolerance ) {}

            /// @name temp
            /// Temperature in kK.
            ///@{
            double& temp()       { clear(); return _temp; }
            double  temp() const { return _temp; }
            ///@}

            /// Returns the wavelength at the maximum intensity in AA.

            double wl_peak() const;

            /// Returns the blackbody responses (F-nu units) at the given wavelength in AA.

            virtual double evaluate( double const wl ) const;

        private :

            double _temp; ///< Temperature in kK.

    };

}

#endif
