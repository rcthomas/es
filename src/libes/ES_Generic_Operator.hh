// 
// File    : ES_Generic_Operator.hh
// --------------------------------
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

#ifndef ES__GENERIC__OPERATOR
#define ES__GENERIC__OPERATOR

namespace ES
{

    namespace Generic
    {

        /// @class Operator
        /// @brief Generic ES Operator interface.
        ///
        /// When instantiated, this template provides an abstract base
        /// Operator class that developers should inherit from to construct
        /// collections of related classes in an elementary supernova (ES)
        /// calculation.  Collections typically include concrete Grid,
        /// Operator, and Setup classes.
        /// 
        /// Operator objects extend Grid object functionality.  Developers
        /// should design Operators so they can be swapped at compile-time
        /// or even at run-time --- this enables a variety of behaviors,
        /// enhances flexibility, and makes testing isolated components
        /// easier.  Operators should have well-defined roles in the
        /// calculation that could be handled by a variety of
        /// implementations.  Operators may keep useful meta-data for a
        /// given calculation (say, solid angle grids to sample at each
        /// radius in the calculation of mean intensity).  An Operator
        /// object should really only attach to a single Grid during its
        /// lifetime.
        /// 
        /// An important note here --- concrete Operator classes that
        /// inherit from the abstract base Operator class instantiated
        /// accept the concrete Grid class as one of their arguments, and
        /// delegate its binding to the abstract base Operator constructor
        /// that Grid.  Do not attempt to bind to the base Grid class
        /// instantiated by ES::Generic::Grid.  This will not work.

        template< typename G, typename S >
            class Operator
            {

                public :

                    /// Constructor taking a concrete Grid object to attach to.

                    Operator( G& grid ) : _grid( &grid ) { _grid->push_operator( *this ); }

                    /// Executes the Setup for the attached Grid object.

                    virtual void operator() ( const S& setup ) = 0;

                protected :

                    /// Reference to the concrete Grid object this Operator is
                    /// attached to.

                    G* _grid;

            };

    }

}

#endif
