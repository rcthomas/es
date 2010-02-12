// 
// File    : ES_Synow_Grid.hh
// --------------------------
// Created : Sun Jan 24 11:29:16 2010
// Authors : Rollin C. Thomas (RCT) - rcthomas@lbl.gov
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

            static Grid create( double const min_spec_wl, double const max_spec_wl, double const bin_step, 
                  int const v_size, double const v_outer_max );

            /// Constructor.

            Grid( double const min_wl_, double const max_wl_, double const bin_step_, int const v_size_ );

            /// Destructor.

            ~Grid();

            /// Zero-out wavelength/velocity axes and opacity/source tables.

            virtual void reset( ES::Synow::Setup& setup );

            double         min_wl;        ///< Bluest wavelength considered in AA.
            double         max_wl;        ///< Reddest wavelength considered in AA.
            double         bin_step;      ///< Opacity/source bin width as Doppler shift ( 1 + dv / c ).
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
