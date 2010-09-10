// $Id: APPSPACK_Parameter_Entry.hpp,v 1.13 2006/06/30 01:42:08 tgkolda Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/APPSPACK_Parameter_Entry.hpp,v $ 

//@HEADER
// ************************************************************************
// 
//          APPSPACK: Asynchronous Parallel Pattern Search
//                 Copyright (2003) Sandia Corporation
// 
// Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
// license for use of this work by or on behalf of the U.S. Government.
// 
// This library is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 2.1 of the
// License, or (at your option) any later version.
//  
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//                                                                                 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.                                                                           .
// 
// Questions? Contact Tammy Kolda (tgkolda@sandia.gov) 
// 
// ************************************************************************
//@HEADER

/*!
  \file APPSPACK_Parameter_Entry.hpp
  \brief Class definition for APPSPACK::Parameter::Entry
*/

#ifndef APPSPACK_PARAMETER_ENTRY_HPP
#define APPSPACK_PARAMETER_ENTRY_HPP

#include "APPSPACK_Common.hpp"		// class data element (string)
#include "APPSPACK_Vector.hpp"
#include "APPSPACK_Matrix.hpp"

namespace APPSPACK {

//! %APPSPACK %Parameter support.
namespace Parameter {

class List; // another parameter type (forward declaration)

/*! 
  Stores a List of parameters \b or a single parameter, which can be
  a bool, int, double, string, APPSPACK::Vector.
  For any single parameter, two attributes are tracked: 
  <ol>
  <li> Used (#isGotten): True if it has been accessed by a "get" function (mutable)
  <li> Default (#isSetByGet): True if it was set by a "get" function
  </ol>

  The "Used" parameter is mutable, which means it can be changed even
  for a const Entry.
 */
class Entry {

public:

  /** \name Constructors / Destructor

  The "Used" parameter is set to false.
  The "Default" parameter is set to false, unless the optional second argument is true.

  */
  //@{ 
  
  //! Create an empty Entry that doesn't contain anything
  Entry();
  
  //! Copy constructor
  Entry(const Entry& source);

  //! Create an Entry containing a bool. 
  Entry(bool value, bool isCreatedByGet = false);

  //! Create an Entry containing an int. 
  Entry(int value, bool isCreatedByGet = false);

  //! Create an Entry containing a double.
  Entry(double value, bool isCreatedByGet = false);

  //! Create an Entry containing a string.
  Entry(const string& value, bool isCreatedByGet = false);

  //! Create an Entry containing a ::Vector. 
  Entry(const Vector& value, bool isCreatedByGet = false);

  //! Destructor
  ~Entry();

  //@}

  //@{ \name Copy

  //! Copy an entry
  Entry& operator=(const Entry& source);

  //@}
  
  /** @name Sublists
   *
   * Functions for handling parameters that are themselves lists.  */
  //@{
  //! Create a sublist. Sets "Default" according to the optional argument. Sets "Used" to false.
  List& setList(bool isCreatedByGet = false);
  //! Extract the reference to the list that is stored in the Entry. Set "Used" to true.
  List& getListValue();
  //! Extract the reference to the list that is stored in the Entry. Set "Used" to true.
  const List& getListValue() const;
  //@}

  /** @name Set functions. 

  Similar to the corresponding constructors. Completely erases the old entry.
  The "Used" parameter is (re-)set to false.
  The "Default" parameter is set to false, unless the optional second argument is true.

  */
  //@{ 
  //! Set bool value
  void setValue(bool value, bool isCreatedByGet = false);
  //! Set int value
  void setValue(int value, bool isCreatedByGet = false);
  //! Set double value
  void setValue(double value, bool isCreatedByGet = false);
  //! Set string value
  void setValue(const char* value, bool isCreatedByGet = false);
  //! Set string value
  void setValue(const string& value, bool isCreatedByGet = false);
  //! Set ::Vector value
  void setValue(const Vector& value, bool isCreatedByGet = false);
  //! Set Matrix value
  void setValue(const Matrix& value, bool isCreatedByGet = false);
  //@}

  /** @name Is functions. 
   
    Return true if the parameter is of the specified type; otherwise,
    return false. Has no affect on "Default" or "Used" status.
  */
  //@{ 
  //! Return true for bool entry
  bool isBool() const;
  //! Return true for int entry
  bool isInt() const;
  //! Return true for double entry
  bool isDouble() const;
  //! Return true for string entry
  bool isString() const;
  //! Return true for list entry (i.e., a sublist)
  bool isList() const;
  //! Return true for ::Vector entry
  bool isVector() const;
  //! Return true for matrix entry
  bool isMatrix() const;
  //@}
  
  /** @name Get functions. 
   
    Returns the value of the Entry. Sets "Used" to true. Does not
    affect "Default" bit.

    \note This will \b not thrown an error if the type of get does not
    match the type of Entry. Use with caution.
  */
  //@{ 
  //! Get bool value
  bool getBoolValue() const;
  //! Get int value
  int getIntValue() const;
  //! Get double value
  double getDoubleValue() const;
  //! Get string value
  const string& getStringValue() const;
  //! Get ::Vector value
  const Vector& getVectorValue() const;
  //! Get Vector value
  const Matrix& getMatrixValue() const;
  //@}

  /** @name Pack/Unpack

  Used by parallel version of APPSPACK to pack and unpack parameter
  lists for message passing. Uses APPSPACK::GCI functionality.

  \note Only works if MPI is enabled at configure time; see \ref
  pageConfigureOptions_MPI.

  */
  //@{
  //! Pack the Entry
  void pack() const;
  //! Unpack the Entry
  void unpack();
  
  //@}

  //@{ \name Printing

  //! Output the parameter to the given stream. 
  /*! 
    Formats the output as "<type,value>", except in the case of a
    list which just outputs "\<sublist\>". If the parameter has not yet
    been set, it outputs "\<NONE\>". This is the function called by the
    ostream operator<<. 
  */
  ostream& leftshift(ostream& stream) const;

  //@}

private:

  //! Reset the entry
  void reset();
  
  //! All possible parameter types that this class can store
  enum EntryType { 
    //! No entry type set yet (will be set later by setValue()
    APPSPACK_NONE, 
    //! Boolean
    APPSPACK_BOOL, 
    //! Integer
    APPSPACK_INT, 
    //! Double
    APPSPACK_DOUBLE, 
    //! String
    APPSPACK_STRING,
    //! Sublist (APPSPACK::Parameter::List)
    APPSPACK_LIST,
    //! APPSPACK::Vector
    APPSPACK_VECTOR,
    //! APPSPACK::Matrix
    APPSPACK_MATRIX
  };

  //! Type of parameter stored in this object.
  EntryType type;

  //! Boolean value, if this is of type BOOL
  bool bval;

  //! Integer value, if this is of type INT
  int ival;

  //! Double value, if this is of type DOUBLE
  double dval;

  //! String value, if this is of type STRING
  string sval;

  //! Pointer to list, if this is of type LIST
  List* lval;		

  //! Vector value
  Vector vectorval;

  //! Matrix value
  Matrix matrixval;

  //! True if this parameter been accessed by a "get" function
  mutable bool isGotten;

  //! True if this parameter is a nominal value assigned by a "get" function
  mutable bool isSetByGet;

};

} // namespace Parameter
} // namespace APPSPACK

//! Output the parameter. Relies of leftshift operator defined in the class.
ostream& operator<<(ostream& stream, const APPSPACK::Parameter::Entry& e);

#endif
