// 
// File    : ES_Line.hh
// --------------------
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
