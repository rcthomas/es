// 
// File    : ES_Generic_Grid.hh
// ----------------------------
// Created : Sun Jan 24 11:23:32 2010
// Authors : Rollin C. Thomas (RCT) - rcthomas@lbl.gov
//  

#ifndef ES__GENERIC__GRID
#define ES__GENERIC__GRID

#include <vector>

namespace ES
{

   namespace Generic
   {

      /// @class Grid
      /// @brief Generic ES grid interface.
      ///
      /// When instantiated, this template provides a base Grid class
      /// that developers should inherit from to construct collections
      /// of related classes in an elementary supernova (ES)
      /// calculation.  Collections typically include concrete Grid,
      /// Operator, and Setup classes.  
      ///
      /// Grid objects contain physical quantities needed for performing
      /// elementary supernova calculations.  Examples of quantities
      /// potentially stored in Grid objects are cached Sobolev
      /// opacities, measurements of mean intensity or line source
      /// functions, escape probabilities, thermalization parameters, and
      /// so on.  The information contained in a Grid could be highly
      /// detailed, or quite schematic --- whatever is appropriate to the
      /// goals of the programmer.
      /// 
      /// Operator objects extend Grid object functionality.  Operators
      /// are attached to Grids effectively at run-time.  Grid objects
      /// are responsible for knowing what Operator objects have been
      /// attached to them, keeping a stack of references to attached
      /// Operators which it executes in succession when a Setup is
      /// passed to it.
      /// 
      /// A useful guideline to consider when defining a concrete Grid
      /// class is what Operators depend on the Grid's data members.  If
      /// only one Operator in the hierarchy needs a particular member,
      /// then that member may actually belong with the Operator instead
      /// of with the Grid object.  The list of members that a Grid is
      /// responsible for should tend towards those that are needed or
      /// modified by more than one Operator.  But, this is just a
      /// guideline --- avoid sacrificing performance or clarity over
      /// principles.

      template< typename O, typename S >
      class Grid
      {

         public :

            /// Add an operator to the stack.

            void push_operator( O& oper ) { _oper.push_back( &oper ); }

            /// Execute a Setup by passing it to each Operator in the
            /// stack in succession.  These Operators use and modify
            /// the Grid.

            void operator() ( S& setup ) 
            { 
               reset( setup );
               for( int i = 0; i < _oper.size(); ++ i ) (*(_oper[ i ] ))( setup ); 
            }

            /// Prepare the Grid for a new calculation, placing it in
            /// a pristine state for attached Operators to work with.
            /// Typically involves zeroing-out temporary tables, or 
            /// re-defining grids.

            virtual void reset( S& setup ) {}

         private :

            /// Stack of references to generic Operator objects.

            std::vector< O* > _oper;

      };

   }

}

#endif
