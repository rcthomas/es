// 
// File    : ES_Synow_Grid.hh
// --------------------------
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
                static Grid create( double const &min_output_wl, double const &max_output_wl, double const &bin_width, 
                        int const v_size, double const * velocities_ );

                /// Constructor.

				Grid(void);
				Grid( const Grid & i_cRHO);
                Grid( double const min_wl_, double const max_wl_, double const bin_width_, int const v_size_ );
                Grid( double const min_wl_, double const max_wl_, double const bin_width_, int const v_size_ , double const * velocities_, bool bRecompute_wl_range = false);

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
				bool           v_user;        ///< Velocity grid was specified by user
                double*        src;           ///< Source function table.
                ES::Blackbody* bb;            ///< Photosphere blackbody function.

            private :

                /// Zero-out wavelength/velocity axes and opacity/source tables.

				void _zeroptr(void);
                void _zero();

        };

    }

}

#endif
