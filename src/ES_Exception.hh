// 
// File    : ES_Exception.hh
// -------------------------
// Created : Fri May 23 18:28:22 2008
// Authors : Rollin C. Thomas (RCT) - rcthomas@lbl.gov
//  

#ifndef ES__EXCEPTION
#define ES__EXCEPTION

#include <stdexcept>

namespace ES
{

   /// @class Exception
   /// @brief Run of the mill exception class.

   class Exception : public std::runtime_error
   {

      public :

         /// Constructor.

         Exception( const std::string& message ) : 
            std::runtime_error( message ) {}

   };

}

#endif
