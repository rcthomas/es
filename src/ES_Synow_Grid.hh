// 
// File    : ES_Synow_Grid.hh
// --------------------------
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

#ifndef ES__SYNOW__GRID
#define ES__SYNOW__GRID

#include "ES_Generic_Grid.hh"

namespace ES
{

   namespace Generic
   {
      template< typename G, typename S >
      class Operator;
   }

   class Blackbody;

   namespace Synow
   {

      class Grid;

      class Setup;

      typedef ES::Generic::Operator< ES::Synow::Grid, ES::Synow::Setup > Operator;

      /// @class Grid
      /// @brief Synow-style implementation of ES::Generic::Grid.
      ///
      /// By "Synow-style" we mean that we keep track of integrated 
      /// Sobolev line opacities and corresponding source functions,
      /// and assume pure resonance scattering.

      class Grid : public ES::Generic::Grid< ES::Synow::Operator, ES::Synow::Setup >
      {

         public :

            /// Named constructor.

            static Grid create( double const min_output_wl, double const max_output_wl, double const bin_width, 
                  int const v_size, double const v_outer_max );

            /// Constructor.

            Grid( double const min_wl_, double const max_wl_, double const bin_width_, int const v_size_ );

            /// Destructor.

            ~Grid();

            /// Zero-out wavelength/velocity axes and opacity/source tables.

            virtual void reset( ES::Synow::Setup& setup );

            double         min_wl;        ///< Bluest wavelength considered in AA.
            double         max_wl;        ///< Reddest wavelength considered in AA.
            double         bin_width;     ///< Opacity/source bin width in kkm/s.
            int            wl_size;       ///< Capacity of wavelength bin array.
            int            wl_used;       ///< Number of wavelength bins with nonzero Sobolev opacity.
            int            v_size;        ///< Line-forming region velocity grid size.
            double*        wl;            ///< Wavelengths of opacity/source bin centers in Angstroms.
            double*        v;             ///< Velocity grid in kkm/s.
            double*        tau;           ///< Sobolev opacity table.
            double*        src;           ///< Source function table.
            ES::Blackbody* bb;            ///< Photosphere blackbody function.

         private :

            /// Zero-out wavelength/velocity axes and opacity/source tables.

            void _zero();

      };

   }

}

#endif
