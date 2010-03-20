// 
// File    : ES_Synapps_Evaluator.hh
// ---------------------------------
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
                        const std::vector< int >& ions,
                        const std::vector< double >& region_weight,
                        const std::vector< double >& region_lower ,
                        const std::vector< double >& region_upper ,
                        double const vector_norm );

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
