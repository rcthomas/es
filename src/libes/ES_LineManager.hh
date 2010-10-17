// 
// File    : ES_LineManager.hh
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

#ifndef ES__LINE_MANAGER
#define ES__LINE_MANAGER

#include "ES_Line.hh"

#include <string>
#include <vector>

namespace ES
{

    /// @class LineManager
    /// @brief Control insertion/removal of lines in a list.
    ///
    /// This class manages lines in a container (an STL container).  Other
    /// types of containers could be added but a vector that one externally
    /// sorts seems to have a lot less overhead in both memory usage and 
    /// speed.  

    class LineManager
    {

        public :

            /// Constructor.

            LineManager( const std::string& line_dir, double const min_wl, double const max_wl ) :
                _line_dir( line_dir ), _min_wl( min_wl ), _max_wl( max_wl ) {}

            /// Destructor.

            ~LineManager() {}

            /// @name load
            /// Insert lines into the list matching a list of ions or an ion.
            ///@{
            void load( const std::vector< int >& ions, std::vector< ES::Line >& lines );
            void load( int const ion, std::vector< ES::Line >& lines );
            ///@}

            /// @name drop
            /// Remove lines from the list matching a list of ions or an ion.
            ///@{
            void drop( const std::vector< int >& ions, std::vector< ES::Line >& lines );
            void drop( int const ion, std::vector< ES::Line >& lines );
            ///@}

        protected :

            std::string _line_dir; ///< Directory where line list files are kept.
            double      _min_wl;   ///< Minimum wavelength to admit to list in AA.
            double      _max_wl;   ///< Maximum wavelength to admit to list in AA.

    };

}

#endif
