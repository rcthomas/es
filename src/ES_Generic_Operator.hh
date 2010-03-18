// 
// File    : ES_Generic_Operator.hh
// --------------------------------
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
