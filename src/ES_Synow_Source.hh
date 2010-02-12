// 
// File    : ES_Synow_Source.hh
// ----------------------------
// Created : Mon Jan 25 14:06:18 2010
// Authors : Rollin C. Thomas (RCT) - rcthomas@lbl.gov
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
      /// @brief TBD
      ///
      /// TBD mention OpenMP

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

            int      _mu_size;
            double*  _mu;
            double*  _dmu;
            double*  _shift;

      };

   }

}

#endif
