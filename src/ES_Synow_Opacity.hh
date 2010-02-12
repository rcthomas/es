// 
// File    : ES_Synow_Opacity.hh
// -----------------------------
// Created : Sun Jan 24 11:31:02 2010
// Authors : Rollin C. Thomas (RCT) - rcthomas@lbl.gov
//  

#ifndef ES__SYNOW__OPACITY
#define ES__SYNOW__OPACITY

#include "ES_Synow_Operator.hh"
#include "ES_LineManager.hh"

#include <vector>
#include <map>

namespace ES
{

   namespace Synow
   {

      class Grid;

      class Setup;

      /// @class Opacity
      /// @brief Synow Sobolev opacity operator.
      ///
      /// TBD

      class Opacity : public ES::Synow::Operator, public ES::LineManager
      {

         public :

            /// Constructor.

            Opacity( ES::Synow::Grid& grid, const std::string& line_dir, const std::string& ref_file,
                  const std::string& form, double const v_ref, double const log_tau_min );

            /// Execute a Setup.

            virtual void operator() ( const ES::Synow::Setup& setup );

         private :

            std::string                _ref_file;     ///< Path to reference line list file.
            std::string                _form;         ///< Functional form of reference line opacity profile.
            double                     _v_ref;        ///< Reference velocity in kkm/s for scaling reference line opacity profiles.
            double                     _log_tau_min;  ///< Minimum Sobolev opacity to include a bin.
            std::map< int, ES::Line >  _ref_lines;    ///< Reference lines.
            std::vector< ES::Line >    _lines;        ///< List of loaded lines.

            /// Drop ions from the line list not needed by the Setup.

            void _drop_ions( const ES::Synow::Setup& setup );

            /// Load ions required by the Setup.

            void _load_ions( const ES::Synow::Setup& setup );


      };

   }

}

#endif
