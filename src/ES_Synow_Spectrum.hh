// 
// File    : ES_Synow_Spectrum.hh
// ------------------------------
// Created : Mon Jan 25 14:06:18 2010
// Authors : Rollin C. Thomas (RCT) - rcthomas@lbl.gov
//  

#ifndef ES__SYNOW__SPECTRUM
#define ES__SYNOW__SPECTRUM

#include "ES_Synow_Operator.hh"

#include <vector>
#include <map>

namespace ES
{

   class Spectrum;

   namespace Synow
   {

      class Grid;

      class Setup;

      /// @class Spectrum
      /// @brief TBD
      /// 
      /// No point in inheriting ES::Spectrum because you would have to
      /// duplicate basically a bunch of named constructors here.
      /// TBD mention OpenMP.

      class Spectrum : public ES::Synow::Operator
      {

         public :

            /// Constructor.

            Spectrum( ES::Synow::Grid& grid, ES::Spectrum& output, int const p_size );

            /// Destructor.

            ~Spectrum();

            /// Execute a Setup.

            virtual void operator() ( const ES::Synow::Setup& setup );

         private :

            ES::Spectrum*  _output;

            int      _p_size;
            int      _p_total;
            double*  _in;
            double*  _p;
            double*  _min_shift;
            double*  _max_shift;

            void _clear();
            void _alloc( bool const clear = true );

      };

   }

}

#endif
