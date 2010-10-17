// 
// File    : ES_Line.hh
// --------------------
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

#ifndef ES__LINE
#define ES__LINE

namespace ES
{

    /// @class Line
    /// @brief Atomic line transition.
    ///
    /// A very basic atomic line transition class, contains the basics
    /// needed for a simple LTE-type calculation.  Therefore, its 
    /// general utility is open to debate.

    class Line
    {

        public :

            /// Constructor.

            Line( int const ion_ = 0, double const wl_ = 0, double const gf_ = 0, double const el_ = 0 ) :
                ion( ion_ ), wl( wl_ ), gf( gf_ ), el( el_ ) {}

            /// Two lines match if they have the same ion code.

            friend bool operator != ( const Line& l, const Line& r )
            { 
                return l.ion != r.ion; 
            }

            /// Lines are to be sorted in increasing order by wavelength.

            friend bool operator < ( const Line& l, const Line& r )
            { 
                return l.wl < r.wl;    
            }

            int    ion; ///< Parent ion code.
            double wl;  ///< Line wavelength in Angstroms.
            double gf;  ///< Oscillator strength.
            double el;  ///< Low-energy level in eV.

    };

}

#endif
