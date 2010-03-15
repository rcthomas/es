// 
// File    : ES_Blackbody.hh
// -------------------------
// Created : Fri Feb  5 15:39:45 2010
// Authors : Rollin C. Thomas (RCT) - rcthomas@lbl.gov
//  

#ifndef ES__BLACKBODY
#define ES__BLACKBODY

#include "ES_Accelerator.hh"

#include <map>

namespace ES
{

   /// @class Blackbody
   /// @brief Blackbody spectral energy distribution.

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

         /// Returns the blackbody responses (F-lambda units) at the given wavelength in AA.

         virtual double evaluate( double const wl ) const;

      private :

         double _temp; ///< Temperature in kK.

   };

}

#endif
