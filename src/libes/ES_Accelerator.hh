// 
// File    : ES_Accelerator.hh
// ---------------------------
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

#ifndef ES__ACCELERATOR
#define ES__ACCELERATOR

#include <map>

namespace ES
{

    /// @class Accelerator
    /// @brief Linear-interpolating cache function accelerator.
    ///
    /// This linear-interpolating cache function accelerator is for 
    /// functions that map a single double-precision float value to 
    /// another single-precision float.  Functions that do that,
    /// such as the Planck blackbody function, can inherit from this
    /// class and fulfill the contract by defining an evaluate()
    /// method.  The operator() method defined in this class uses
    /// the caching mechanism; a subclass evaluate() method should
    /// not.
    ///
    /// Most other implementations of lookup-tables or accelerators
    /// have interfaces that I don't like.  This implementation does
    /// not force the user to supply a pre-sorted array of abscissae
    /// to initialize --- it handles sorting itself.  The internals
    /// use an STL map, so the lookup-table is extensible.  In fact,
    /// extending the lookup-table automatically triggers recursive
    /// addition of reference values in the cache.  This recursive
    /// insertion is driven by relative error estimates, so the 
    /// cache concentrates abscissae where the function varies a lot
    /// and not so much where it is smooth and linear.  One could 
    /// envision a lot of enhancements here, such as higher-order
    /// interpolation or using a real adaptive mesh, but so far this
    /// implementation satisfies me.

    class Accelerator
    {

        public :

            /// Constructor.

            Accelerator( double const tolerance ) :
                _tolerance( tolerance ) {}

            /// Empty the cache.

            void clear() { _cache.clear(); }

            /// Returns a response for a single input.

            double operator() ( double const x );

            /// Subclasses implement this method to return a response.

            virtual double evaluate( double const x ) const = 0;

        private :

            double                     _tolerance;    ///< Maximum relative error allowed at a cached abcissa.
            std::map< double, double > _cache;        ///< Cached input-response pairs.

            /// Recursively inserts input-response pairs into the cache.

            void _insert( double const x, double const y );

    };

}

#endif
