// 
// File    : ES_Synapps_Evaluator.hh
// ---------------------------------
// Created : Tue Feb 23 16:13:54 2010
// Authors : Rollin C. Thomas (RCT) - rcthomas@lbl.gov
// 

#ifndef ES__SYNAPPS__EVALUATOR
#define ES__SYNAPPS__EVALUATOR

#include "APPSPACK_Evaluator_Interface.hpp"

namespace APPSPACK
{
   class Vector;
}

namespace ES
{

   class Spectrum;

   namespace Synow
   {

      class Grid;

      class Setup;

   }

   namespace Synapps
   {

      class Grid;

      /// @class Evaluator
      /// @brief Synow-style APPSPACK evaluator implementation.

      class Evaluator : APPSPACK::Evaluator::Interface
      {

         public :

            /// Constructor.

            Evaluator( ES::Synow::Grid& grid, ES::Spectrum& target, ES::Spectrum& output, 
                    const std::vector< int >& ions, double const vector_norm );

            /// Do the function evaluation for the input point and fill in the result.

            virtual void operator() ( int tag, const APPSPACK::Vector& x, APPSPACK::Vector& f, std::string& msg );

            /// Prints information about the evaluator object.

            virtual void print() const
            {
               std::cout << "Hello" << std::endl;
            }

         private :

            ES::Synow::Setup*  _setup;      ///< Elementary supernova setup.
            ES::Synow::Grid*   _grid;       ///< Elementary supernova grid.
            ES::Spectrum*      _target;     ///< Target spectrum to be fit.
            ES::Spectrum*      _output;     ///< Synthesized spectrum fit to the target.

            double   _vector_norm;          ///< Norm between observed and synthesized spectrum.

      };

   }

}

#endif
