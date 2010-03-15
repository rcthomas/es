// 
// File    : ES_Line.hh
// --------------------
// Created : Thu 27 Mar 2008 01:27:13 PM PDT
// Authors : Rollin C. Thomas (RCT) - rcthomas@lbl.gov
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
