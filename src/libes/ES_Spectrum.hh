// 
// File    : ES_Spectrum.hh
// ------------------------
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

#ifndef ES__SPECTRUM
#define ES__SPECTRUM

#include <string>
#include <vector>
#include <iostream>

namespace ES
{

    /// @class Spectrum
    /// @brief Represents a supernova spectrum.
    ///
    /// This class is not designed to handle every possible manipulation you
    /// may want to perform on a supernova spectrum, and it is probably not 
    /// what you want for an abstract astrophysical source spectrum.  It is 
    /// assumed that when this class is used to ingest an observed spectrum
    /// for analysis that it has been pre-standardized (in that it has been
    /// corrected for Milky Way extinction, host galaxy redshift, and maybe
    /// host galaxy extinction, and so on).  
    ///
    /// A number of convenient named constructors are provided as static 
    /// methods returning an ES::Spectrum object.  Also note that stream 
    /// operators are included as well.
    ///
    /// The basic idea is that a spectrum is a list of 3-tuples with contents
    /// (wavelength, flux, flux-error).  This list is to be sorted by the 
    /// wavelength entry in ascending order.  Access to elements of tuples is
    /// managed through an interface.

    class Spectrum
    {

        public :

            /// Constructor taking a size argument.  A zeroed-out spectrum
            /// (all tuples having 0 for all entries) is returned.

            static Spectrum create_from_size( size_t const size );

            /// Constructor taking wavelength range and step arguments, in
            /// Angstroms.  A spectrum is returned with flux and flux-error
            /// columns zeroed-out (both columns having 0 for all entries).
            /// The first wavelength value is guaranteed to equal the min_wl 
            /// argument, but the last wavelength value is only guaranteed
            /// to not exceed max_wl.

            static Spectrum create_from_range_and_step( double const min_wl, double const max_wl, double const wl_step );

            /// Constructor taking wavelength range (in Angstroms) and size 
            /// arguments.  A spectrum is returned with flux and flux-error
            /// columns zeroed-out (both columns having 0 for all entries).
            /// The first and last wavelength values are guaranteed to
            /// equal min_wl and max_wl respectively.

            static Spectrum create_from_range_and_size( double const min_wl, double const max_wl, size_t const size );

            /// Constructor taking a path to a multicolumn ASCII file.  The 
            /// file contents are parsed and used to initialize a spectrum 
            /// which is returned.

            static Spectrum create_from_ascii_file( const char* file );

            /// Constructor taking a path to a FITS spectrum file.  The 
            /// file contents are parsed and used to initialize a spectrum 
            /// which is returned.  May not work with every kind of spectrum.
            /// (I hate including this method.)

            static Spectrum create_from_fits_file( const char* file );

            /// Constructor taking another spectrum, from which a zeroed out
            /// spectrum of the same size and wavelength axis is initialized.

            static Spectrum create_from_spectrum( const ES::Spectrum& original );

            /// Number of (wavelength, flux, flux-error) tuples.

            size_t size() const { return _wl.size(); }

            /// Adjust spectrum size, adding or deleting tuples.  Note that 
            /// in lieu of trying to handle the addition of points to the 
            /// beginning, middle, or end of the spectrum, the spectrum is 
            /// zeroed-out.

            void resize( size_t const size );

            /// Set the size of the spectrum to zero.  Note that this is not
            /// the same operation as zero_out().

            void clear();

            /// Returns true if the spectrum has size zero.

            bool is_empty() const { return _wl.size() == 0; }

            /// Set all tuple entries in the spectrum to zero.  This is 
            /// equivalent to resizing the spectrum to its current size.

            void zero_out();

            /// Rescale the fluxes and flux errors so that the median is 
            /// set to the desired value.

            void rescale_median_flux( double const median = 1.0 );

            /// @name Wavelength Methods
            /// Methods for accessing wavelength values, probably in units of 
            /// AA.

            //@{

            /// Assign wavelength value to spectrum tuple position i.
            double& wl( size_t const i )       { return _wl[ i ]; }

            /// Return wavelength value at spectrum tuple position i.
            double  wl( size_t const i ) const { return _wl[ i ]; }

            /// Minimum wavelength in the spectrum.
            double min_wl() const { return _wl.front(); }

            /// Maximum wavelength in the spectrum.
            double max_wl() const { return _wl.back(); }

            //@}

            /// @name Flux Accessors
            /// Methods for accessing flux values, probably in rescaled 
            /// \f$F_\lambda\f$ units.

            //@{

            /// Assign flux value to spectrum tuple position i.
            double& flux( size_t const i )       { return _flux[ i ]; }

            /// Return flux value at spectrum tuple position i.
            double  flux( size_t const i ) const { return _flux[ i ]; }

            /// Minimum flux in the spectrum.
            double min_flux() const;

            /// Maximum flux in the spectrum.
            double max_flux() const;

            //@}

            /// @name Flux Error Accessors
            /// Methods for accessing flux-error values, probably in rescaled 
            /// F_lambda units.  If the errors are purely statistical, this 
            /// corresponds to the 1-sigma standard deviation.

            //@{

            /// Assign flux-error value to spectrum tuple position i.
            double& flux_error( size_t const i )       { return _flux_error[ i ]; }

            /// Return flux-error value at spectrum tuple position i.
            double  flux_error( size_t const i ) const { return _flux_error[ i ]; }

            //@}

            /// @name I/O stream operators.

            //@{

            /// Input stream operator.
            friend std::istream& operator >> ( std::istream& stream, ES::Spectrum& spectrum );

            /// Output stream operator.
            friend std::ostream& operator << ( std::ostream& stream, const ES::Spectrum& spectrum );

            //@}

        private :

            std::vector< double >  _wl;         ///< Wavelengths in AA.
            std::vector< double >  _flux;       ///< Flux values in erg / cm^2 / s / AA.
            std::vector< double >  _flux_error; ///< Flux errors in erg / cm^2 / s / AA.

    };

}

#endif
