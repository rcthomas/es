// 
// File    : ES_Synow_Setup.hh
// ---------------------------
// Created : Mon Dec 14 12:24:38 2009
// Authors : Rollin C. Thomas (RCT) - rcthomas@lbl.gov
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
