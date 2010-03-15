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
      /// @brief Computes a pure resonance scattering source function.
      ///
      /// This operator computes the pure-resonance scattering line source
      /// functions for opacity bins in the Grid.  As this is the most 
      /// numerically intensive part of the calculation, OpenMP-support is
      /// enabled if it is detected at compile-time.  This calculation is
      /// performed using the familiar ray-tracing method --- not a Monte 
      /// Carlo calculation.  So, while it is faster it is not accurate 
      /// enough to be used for a radiative equilibrium calculation.

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

            // Note that the full compliment of angles at each point is 
            // 2 * mu_size --- one set is for rays subtending the sky 
            // and the other set is for rays subtending the photosphere.
            // This implementation is actually a good compromise of speed
            // and accuracy, and was first used in the original Synow.
            // The resolution requirements are pretty lax here.

            int      _mu_size;      ///< Number of angles subtending either sky or photosphere.
            double*  _mu;           ///< List of angles as direction-cosines.
            double*  _dmu;          ///< Step in direction-cosine units for integral.
            double*  _shift;        ///< Minimum Doppler first-order Doppler shift along each ray.

      };

   }

}

#endif
