// 
// File    : ES_Synow_VectorSource.hh
// ----------------------------------
// Created : Fri Feb 19 15:10:03 2010
// Authors : Rollin C. Thomas (RCT) - rcthomas@lbl.gov
//  

#ifndef ES__SYNOW__VECTOR_SOURCE
#define ES__SYNOW__VECTOR_SOURCE

#include "ES_Synow_Operator.hh"

#include <vector>
#include <map>

namespace ES
{

   namespace Synow
   {

      class Grid;

      class Setup;

      /// @class VectorSource
      /// @brief TBD
      ///
      /// TBD mention OpenMP

      class VectorSource : public ES::Synow::Operator
      {

         public :

            /// Constructor.

            VectorSource( ES::Synow::Grid& grid, int const mu_size );

            /// Destructor.

            ~VectorSource();

            /// Execute a Setup.

            virtual void operator() ( const ES::Synow::Setup& setup );

         private :

            int      _mu_size;
            int*     _iv;
            bool*    _phot;
            int*     _start;
            double*  _v;
            double*  _in;
            double*  _mu;
            double*  _dmu;
            double*  _shift;

      };

   }

}

#endif
