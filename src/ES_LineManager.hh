// 
// File    : ES_LineManager.hh
// ---------------------------
// Created : Thu 27 Mar 2008 04:01:18 PM PDT
// Authors : Rollin C. Thomas (RCT) - rcthomas@lbl.gov
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
