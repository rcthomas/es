// 
// File    : ES_Synapps_Evaluator.hh
// ---------------------------------
//
// ES: Elementary Supernova Spectrum Synthesis, Copyright (c) 2010, The
// Regents of the University of California, through Lawrence Berkeley
// National Laboratory (subject to receipt of any required approvals
// from the U.S. Dept. of Energy). All rights reserved.
//
// If you have questions about your rights to use or distribute this
// software, please contact Berkeley Lab's Technology Transfer
// Department at TTD@lbl.gov.
//
// NOTICE. This software was developed under partial funding from the
// U.S. Department of Energy. As such, the U.S. Government has been
// granted for itself and others acting on its behalf a paid-up,
// nonexclusive, irrevocable, worldwide license in the Software to
// reproduce, prepare derivative works, and perform publicly and display
// publicly. Beginning five (5) years after the date permission to
// assert copyright is obtained from the U.S. Department of Energy, and
// subject to any subsequent five (5) year renewals, the U.S. Government
// is granted for itself and others acting on its behalf a paid-up,
// nonexclusive, irrevocable, worldwide license in the Software to
// reproduce, prepare derivative works, distribute copies to the public,
// perform publicly and display publicly, and to permit others to do so. 
//

#ifndef ES__SYNAPPS__EVALUATOR
#define ES__SYNAPPS__EVALUATOR

#include <appspack/APPSPACK_Evaluator_Interface.hpp>

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

                virtual void print() const {}

            private :

                ES::Synow::Setup*       _setup;         ///< Elementary supernova setup.
                ES::Synow::Grid*        _grid;          ///< Elementary supernova grid.
                ES::Spectrum*           _target;        ///< Target spectrum to be fit.
                ES::Spectrum*           _output;        ///< Synthesized spectrum fit to the target.

                double                  _vector_norm;   ///< Norm between observed and synthesized spectrum.
                std::vector< double >   _weight;        ///< Weight vector for objective function.

        };

    }

}

#endif
