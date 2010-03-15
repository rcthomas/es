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
      /// @brief Computes a spectrum.
      /// 
      /// No point in inheriting ES::Spectrum because you would have to
      /// duplicate basically a bunch of named constructors here.

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

            ES::Spectrum*  _output;     ///< Synthetic spectrum object.

            int      _p_size;           ///< Number of impact parameters subtending photosphere.
            int      _p_total;          ///< Total number of impact parameters subtending line-forming region.
            double*  _in;               ///< Specific intensities.
            double*  _p;                ///< Impact parameters.
            double*  _min_shift;        ///< Minimum first-order Doppler shift along each impact parameter.
            double*  _max_shift;        ///< Maximum first-order Doppler shift along each impact parameter.

            // Note the presence of the following _alloc() and _clear()
            // pair of methods.  Unlike the other operators in this 
            // namespace, we occasionally need to re-allocate some of 
            // the internal memory when executing an ES::Synow::Setup, 
            // because the size of the line forming region may change
            // between calls to the ES::Synow::Grid.  This triggers a
            // change in the number of impact parameters.  This should
            // only result in enlargements of the array, and is done 
            // only when necessary.

            /// Allocate memory.

            void _alloc( bool const clear = true );

            /// Deallocate memory.

            void _clear();

      };

   }

}

#endif
