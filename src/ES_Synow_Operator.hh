// 
// File    : ES_Synow_Operator.hh
// ------------------------------
// Created : Tue Feb  2 15:22:07 2010
// Authors : Rollin C. Thomas (RCT) - rcthomas@lbl.gov
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
