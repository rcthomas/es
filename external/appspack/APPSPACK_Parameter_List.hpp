// $Id: APPSPACK_Parameter_List.hpp,v 1.14 2006/06/30 01:42:08 tgkolda Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/APPSPACK_Parameter_List.hpp,v $ 

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
  \file APPSPACK_Parameter_List.hpp
  \brief Class definition for APPSPACK::Parameter::List
*/

#ifndef APPSPACK_PARAMETER_LIST_HPP
#define APPSPACK_PARAMETER_LIST_HPP

#include "APPSPACK_Common.hpp"
#include "APPSPACK_Parameter_Entry.hpp"
#include "APPSPACK_GCI.hpp"
#include "APPSPACK_Matrix.hpp"

namespace APPSPACK {
namespace Parameter {

/*! A List stores a set of parameters, optionally organized in
  sublists. Each parameter has a name and an Entry. The Entry is
  itself a List (i.e., a sublist) or a bool, int, double, string,
  Value, or ::Vector. Each Entry keeps track of
  whether or not it was set to be the default value or set by the
  input and whether or not it has been used. */

class List {

  //! Parameter container typedef
  typedef map<string, Entry> Map;

  //! Parameter container const iterator typedef
  typedef Map::const_iterator ConstIterator;

  //! Parameter container iterator typedef
  typedef Map::iterator Iterator;

public:


    //! Constructor
  List();

  //! Copy constructor (deep copy)
  List(const List& source);

  //! Copy (deep copy)
  List& operator=(const List& source);

  //! Destructor
  ~List();

  //-------------------------
  /** @name Sublists 

  The entries in a parameter list can be organized into sublists.

  */
  //@{

  //! Create a new sublist \b or return the sublist if it already exists.
  /*! 
    Creates and empty sublist and returns a reference to the
    sublist. If the list already exists, returns reference to that
    sublist. If the name exists but is not a sublist, throws an error.
  */
  List& sublist(const string& name);

  //! Returns a const reference to the sublist if it exists, otherwise throw an error.
  /*! 
    If the list does not already exist, throws an error. If the name
    exists but is not a sublist, throws an error.
  */
  const List& sublist(const string& name) const;
  //@}
  

  //-------------------------
  /** @name Setting Parameters 

    Methods to set parameters in a List. Each parameter has a name and
    a value which is either bool, int, double, string, Value, or
    ::Vector. 

    Be sure to use static_cast<type>() when the type is
    ambiguous. For example,

    \code
    List list;
    list.setParameter("Blah", static_cast<double>(0))
    \endcode

    creates a double parameter named "Blah" with a value of zero.

    Both char* and string are stored as string's
    internally. 

    Sets "Used" to false and "Default" to false.
  */
  //@{

  //! Set a bool parameter
  void setParameter(const string& name, bool value);

  //! Set an int parameter
  void setParameter(const string& name, int value);

  //! Set a double parameter
  void setParameter(const string& name, double value);

  //! Set a string parameter
  void setParameter(const string& name, const char* value);

  //! Set a string parameter
  void setParameter(const string& name, const string& value);

  //! Set an ::Vector parameter
  void setParameter(const string& name, const Vector& value);

  //! Set an ::Vector parameter
  void setParameter(const string& name, const Matrix& value);
  //@}

  /** @name Getting Parameters 
   
    Get the value of a parameter from the list. Returns the nominal
    value if the parameter is not already specified. Sets "Used" to
    true.

    The non-const version adds the nominal value to the list if it's
    not already specified.  In this case, "Default" is set to true.

    Use static_cast<type>() when the type is
    ambiguous. 

    Both char* and string map return string values.
  */
  //@{

  //! Get a bool parameter
  bool getParameter(const string& name, bool nominal);

  //! Get an int parameter
  int getParameter(const string& name, int nominal);

  //! Get a double parameter
  double getParameter(const string& name, double nominal);

  //! Get a string parameter
  const string& getParameter(const string& name, const char* nominal);

  //! Get a string parameter
  const string& getParameter(const string& name, const string& nominal);

  //! Get an ::Vector parameter
  const Vector& getParameter(const string& name, const Vector& nominal);


  //! Get a bool parameter - no change to the list
  bool getParameter(const string& name, bool nominal) const;

  //! Get an int parameter - no change to the list
  int getParameter(const string& name, int nominal) const;

  //! Get a double parameter - no change to the list
  double getParameter(const string& name, double nominal) const;

  //! Get a string parameter - no change to the list
  const string& getParameter(const string& name, const char* nominal) const;

  //! Get a string parameter - no change to the list
  const string& getParameter(const string& name, const string& nominal) const;

  //! Get an ::Vector parameter - no change to the list
  const Vector& getParameter(const string& name, const Vector& nominal) const;

  //@}

  /** @name Getting Parameters Without Nominal Value

  Get the value of a parameter from the list. Turns "Used" to
  true. Throws an error if the parameter does not exist or is not the
  correct type.
  */
  //@{

  //! Get a double parameter - no change to the list
  double getDoubleParameter(const string& name) const;

  //! Get an APPSPACK::Vector parameter - no change to the list
  const Vector& getVectorParameter(const string& name) const;

  //! Get Matrix value
  const Matrix& getMatrixParameter(const string& name) const;
  //@}
 
  /** @name Checking parameter existence.
   
    Returns true if the specified parameter exists AND is of the
    specified type.
  */
  //@{

  //! Return true if a parameter with this name exists.
  bool isParameter(const string& name) const;

  //! Return true if a bool parameter with this name exists
  bool isParameterBool(const string& name) const;

  //! Return true if an int parameter with this name exists
  bool isParameterInt(const string& name) const;

  //! Return true if a double parameter with this name exists
  bool isParameterDouble(const string& name) const;

  //! Return true if a string parameter with this name exists
  bool isParameterString(const string& name) const;

  //! Return true if a sublist with this name exists
  bool isParameterSublist(const string& name) const;

  //! Return true if a Value parameter with this name exists
  bool isParameterValue(const string& name) const;

  //! Return true if a ::Vector parameter with this name exists
  bool isParameterVector(const string& name) const;

  //! Return true if a matrix parameter with this name exists
  bool isParameterMatrix(const string& name) const;
  //@}

  /** @name Is Parameter Equal Value?
   
    Returns true if the specified parameter exists AND is equal to the
    specified value.  
  */
  //@{

  //! Return true if a bool parameter with this name exists and is equal to the specified value
  bool isParameterEqual(const string& name, bool value) const;

  //! Return true if an int parameter with this name exists and is equal to the specified value
  bool isParameterEqual(const string& name, int value) const;

  //! Return true if a double parameter with this name exists and is equal to the specified value
  bool isParameterEqual(const string& name, double value) const;

  //! Return true if a string parameter with this name exists and is equal to the specified value
  bool isParameterEqual(const string& name, const char* value) const;

  //! Return true if a string parameter with this name exists and is equal to the specified value
  bool isParameterEqual(const string& name, const string& value) const;

  //! Return true if a ::Vector parameter with this name exists and is equal to the specified value
  bool isParameterEqual(const string& name, const Vector& value) const;

  //@}

  //@{ \name Printing

  //! Pretty-print a list. Indents sublists.
  ostream& print(ostream& stream = cout, int indent = 0) const;

  //@}

  /** @name Pack/Unpack for Communication

  \note Only works if MPI is enabled at configure time; see \ref
  pageConfigureOptions_MPI.
  */

  //@{ 
    
  //! Pack
  void pack() const;

  //! Unpack
  void unpack();

  //@}

private:

  //! Check to see if "l" or any of its sublists is "this"
  bool isRecursive(const List& l) const;

  //! Access to name (i.e., returns i->first)
  const string& name(ConstIterator i) const;

  //! Access to Entry (i.e., returns i->second)
  Entry& entry(Iterator i);

  //! Access to Entry (i.e., returns i->second)
  const Entry& entry(ConstIterator i) const;

private:

  //! Enums for pack/unpack
  enum PackingCodes 
  {
    //! Indicates that another entry is packed
    NEW_ENTRY,
    //! Indicates the end of the packed list
    END_OF_LIST
  };

  //! Parameter list
  Map params;
 
  //! Used to create a string when the getParameter is called with a
  //! char* nominal value. A new string is created for each such
  //! argument. The whole group of strings is destroyed when this object
  //! is destroyed. This is really annoying, but I don't know a better 
  //! way.
  mutable vector<string> tmpstrings;
};
}
}

#endif


