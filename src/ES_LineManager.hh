// 
// File    : ES_LineManager.hh
// ---------------------------
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
