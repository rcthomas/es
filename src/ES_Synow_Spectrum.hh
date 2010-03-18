// 
// File    : ES_Synow_Spectrum.hh
// ------------------------------
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

                Spectrum( ES::Synow::Grid& grid, ES::Spectrum& output, int const p_size, bool const flatten );

                /// Destructor.

                ~Spectrum();

                /// Execute a Setup.

                virtual void operator() ( const ES::Synow::Setup& setup );

            private :

                ES::Spectrum*  _output;     ///< Synthetic spectrum object.

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
