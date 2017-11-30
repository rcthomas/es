// 
// File    : ES_Synow_Spectrum.hh
// ------------------------------
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

                Spectrum( ES::Synow::Grid& grid, ES::Spectrum& output, ES::Spectrum& reference, int const p_size, bool const flatten );

				/// Copy Constructor

                Spectrum( const ES::Synow::Spectrum& i_cRHO);

                /// Destructor.

                ~Spectrum();

                /// Execute a Setup.

                virtual void operator() ( const ES::Synow::Setup& setup );

            private :

                ES::Spectrum*  _output;     ///< Synthetic spectrum object.
                ES::Spectrum*  _reference;  ///< Reference pseudo-continuum for flattening.

                bool _flatten;              ///< If true, "flatten" the spectrum.

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
