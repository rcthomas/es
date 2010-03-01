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
      /// @brief TODO
      ///
      /// TODO

      class Evaluator : APPSPACK::Evaluator::Interface
      {

         public :

            /// Constructor.

            Evaluator( ES::Synow::Grid& grid, ES::Spectrum& target, ES::Spectrum& output, const std::vector< int >& ions, double const vector_norm );

            /// Do the function evaluation for the input point and fill in the result.

            virtual void operator() ( int tag, const APPSPACK::Vector& x, APPSPACK::Vector& f, std::string& msg );

//          virtual void print() const;

         private :

            ES::Synow::Setup*  _setup;
            ES::Synow::Grid*   _grid;
            ES::Spectrum*      _target;
            ES::Spectrum*      _output;

            double   _vector_norm;

      };

   }

}

#endif
