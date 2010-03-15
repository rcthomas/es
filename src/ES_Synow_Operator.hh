// 
// File    : ES_Synow_Operator.hh
// ------------------------------
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

#ifndef ES__SYNOW__OPERATOR
#define ES__SYNOW__OPERATOR

#include "ES_Generic_Operator.hh"

namespace ES
{

   namespace Synow
   {

      class Grid;

      class Setup;

      /// @var   Operator
      /// @brief Abstract base class for Synow-style calculations.

      typedef ES::Generic::Operator< ES::Synow::Grid, ES::Synow::Setup > Operator;

   }

}

#endif
