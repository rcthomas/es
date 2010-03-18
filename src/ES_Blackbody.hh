// 
// File    : ES_Blackbody.hh
// -------------------------
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
