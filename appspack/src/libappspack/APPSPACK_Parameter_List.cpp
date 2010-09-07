// $Id: APPSPACK_Parameter_List.cpp,v 1.17 2006/06/30 01:42:08 tgkolda Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/APPSPACK_Parameter_List.cpp,v $ 

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
  \file APPSPACK_Parameter_List.cpp
  \brief Implementation of APPSPACK::Parameter::List
*/

#include "APPSPACK_Parameter_List.hpp"	

APPSPACK::Parameter::List::List() {}

APPSPACK::Parameter::List::List(const List& source) 
{
  params = source.params;
}

APPSPACK::Parameter::List& APPSPACK::Parameter::List::operator=(const List& source) 
{
  if (&source == this)
    return *this;

  params = source.params;
  return *this;
}

APPSPACK::Parameter::List::~List() 
{
}

void APPSPACK::Parameter::List::setParameter(const string& name, bool value)
{
  params[name].setValue(value);
}

void APPSPACK::Parameter::List::setParameter(const string& name, int value)
{
  params[name].setValue(value);
}

void APPSPACK::Parameter::List::setParameter(const string& name, double value)
{
  params[name].setValue(value);
}

void APPSPACK::Parameter::List::setParameter(const string& name, const char* value)
{
  params[name].setValue(value);
}

void APPSPACK::Parameter::List::setParameter(const string& name, const string& value)
{
  params[name].setValue(value);
}

void APPSPACK::Parameter::List::setParameter(const string& name, const Vector& value)
{
  params[name].setValue(value);
}

void APPSPACK::Parameter::List::setParameter(const string& name, const Matrix& value)
{
  params[name].setValue(value);
}

bool APPSPACK::Parameter::List::getParameter(const string& name, bool nominal)
{
  ConstIterator i = params.find(name);

  if (i == params.end()) {
    params[name].setValue(nominal, true);
    i = params.find(name);
  }

  if ((i != params.end()) && (entry(i).isBool()))
    return entry(i).getBoolValue();

  cerr << "APPSPACK::Parameter::List::getParameter - get error for bool" << endl;
  throw "APPSPACK Error";
}

int APPSPACK::Parameter::List::getParameter(const string& name, int nominal) 
{
  ConstIterator i = params.find(name);

  if (i == params.end()) {
    params[name].setValue(nominal, true);
    i = params.find(name);
  }

  if ((i != params.end()) && (entry(i).isInt()))
    return entry(i).getIntValue();

  cerr << "APPSPACK::Parameter::List::getParameter - get error for int" << endl;
  throw "APPSPACK Error";
}

double APPSPACK::Parameter::List::getParameter(const string& name, double nominal) 
{
  ConstIterator i = params.find(name);

  if (i == params.end()) {
    params[name].setValue(nominal, true);
    i = params.find(name);
  }

  if ((i != params.end()) && (entry(i).isDouble()))
    return entry(i).getDoubleValue();

  cerr << "APPSPACK::Parameter::List::getParameter - get error for double" << endl;
  throw "APPSPACK Error";

}

const string& APPSPACK::Parameter::List::getParameter(const string& name, const char* nominal) 
{
  ConstIterator i = params.find(name);

  if (i == params.end()) {
    params[name].setValue(nominal, true);
    i = params.find(name);
  }

  if ((i != params.end()) && (entry(i).isString()))
    return entry(i).getStringValue();

  cerr << "APPSPACK::Parameter::List::getParameter - get error for string" << endl;
  throw "APPSPACK Error";
}

const string& APPSPACK::Parameter::List::getParameter(const string& name, const string& nominal) 
{
  ConstIterator i = params.find(name);

  if (i == params.end()) {
    params[name].setValue(nominal, true);
    i = params.find(name);
  }

  if ((i != params.end()) && (entry(i).isString()))
    return entry(i).getStringValue();

  cerr << "APPSPACK::Parameter::List::getParameter - get error for string" << endl;
  throw "APPSPACK Error";
}
  
const APPSPACK::Vector& APPSPACK::Parameter::List::getParameter(const string& name, const Vector& nominal) 
{
  ConstIterator i = params.find(name);

  if (i == params.end()) 
  {
    params[name].setValue(nominal, true);
    i = params.find(name);
  }

  if ((i != params.end()) && (entry(i).isVector()))
    return entry(i).getVectorValue();

  cerr << "APPSPACK::Parameter::List::getParameter - get error for Vector" << endl;
  throw "APPSPACK Error";
}
  
bool APPSPACK::Parameter::List::getParameter(const string& name, bool nominal) const
{
  ConstIterator i = params.find(name);
  if ((i != params.end()) && (entry(i).isBool()))
    return entry(i).getBoolValue();
  return nominal;
}

int APPSPACK::Parameter::List::getParameter(const string& name, int nominal) const
{
  ConstIterator i = params.find(name);
  if ((i != params.end()) && (entry(i).isInt()))
    return entry(i).getIntValue();
  return nominal;
}

double APPSPACK::Parameter::List::getParameter(const string& name, double nominal) const
{
  ConstIterator i = params.find(name);
  if ((i != params.end()) && (entry(i).isDouble()))
    return entry(i).getDoubleValue();
  return nominal;
}

const string& APPSPACK::Parameter::List::getParameter(const string& name, const char* nominal) const
{
  ConstIterator i = params.find(name);
  if ((i != params.end()) && (entry(i).isString()))
    return entry(i).getStringValue();

  // Save nominal char* value as a string, and return the string value.
  tmpstrings.push_back(nominal);
  return tmpstrings.back();
}

const string& APPSPACK::Parameter::List::getParameter(const string& name, const string& nominal) const
{
  ConstIterator i = params.find(name);
  if ((i != params.end()) && (entry(i).isString()))
    return entry(i).getStringValue();
  return nominal;
}
  
const APPSPACK::Vector& APPSPACK::Parameter::List::getParameter(const string& name, const Vector& nominal) const
{
  ConstIterator i = params.find(name);
  if ((i != params.end()) && (entry(i).isVector()))
    return entry(i).getVectorValue();
  return nominal;
}
  

double APPSPACK::Parameter::List::getDoubleParameter(const string& name) const
{
  ConstIterator i = params.find(name);

  if ((i != params.end()) && (entry(i).isDouble()))
    return entry(i).getDoubleValue();

  cerr << "APPSPACK::Parameter::List::getValueParameter - no such parameter (" << name << ")"<< endl;
  throw "APPSPACK Error";
}
  
const APPSPACK::Vector& APPSPACK::Parameter::List::getVectorParameter(const string& name) const
{
  ConstIterator i = params.find(name);

  if ((i != params.end()) && (entry(i).isVector()))
    return entry(i).getVectorValue();

  cerr << "APPSPACK::Parameter::List::getVectorParameter - no such parameter (" << name << ")" << endl;
  throw "APPSPACK Error";
}

const APPSPACK::Matrix& APPSPACK::Parameter::List::getMatrixParameter(const string& name) const
{
  ConstIterator i = params.find(name);

  if ((i != params.end()) && (entry(i).isMatrix()))
    return entry(i).getMatrixValue();

  cerr << "APPSPACK::Parameter::List::getMatrixParameter - no such parameter (" << name << ")" << endl;
  throw "APPSPACK Error";
}

bool APPSPACK::Parameter::List::isParameterBool(const string& name) const
{
  ConstIterator i = params.find(name);

  if (i != params.end())
    return (entry(i).isBool());

  return false;
}

bool APPSPACK::Parameter::List::isParameterInt(const string& name) const
{
  ConstIterator i = params.find(name);

  if (i != params.end())
    return (entry(i).isInt());

  return false;
}

bool APPSPACK::Parameter::List::isParameterDouble(const string& name) const
{
  ConstIterator i = params.find(name);

  if (i != params.end())
    return (entry(i).isDouble());

  return false;
}

bool APPSPACK::Parameter::List::isParameterString(const string& name) const
{
  ConstIterator i = params.find(name);

  if (i != params.end())
    return (entry(i).isString());

  return false;
}

bool APPSPACK::Parameter::List::isParameterSublist(const string& name) const
{
  ConstIterator i = params.find(name);

  if (i != params.end())
    return (entry(i).isList());

  return false;
}

bool APPSPACK::Parameter::List::isParameterVector(const string& name) const
{
  ConstIterator i = params.find(name);

  if (i != params.end())
    return (entry(i).isVector());

  return false;
}

bool APPSPACK::Parameter::List::isParameterMatrix(const string& name) const
{
  ConstIterator i = params.find(name);

  if (i != params.end())
    return (entry(i).isMatrix());

  return false;
}

bool APPSPACK::Parameter::List::isParameter(const string& name) const
{
  return (params.find(name) != params.end());
}

bool APPSPACK::Parameter::List::isParameterEqual(const string& name, bool value) const
{
  ConstIterator i = params.find(name);
  if ((i != params.end()) && (entry(i).isBool()))
    return (entry(i).getBoolValue() == value);
  return false;
}

bool APPSPACK::Parameter::List::isParameterEqual(const string& name, int value) const
{
  ConstIterator i = params.find(name);
  if ((i != params.end()) && (entry(i).isInt()))
    return (entry(i).getIntValue() == value);
  return false;
}

bool APPSPACK::Parameter::List::isParameterEqual(const string& name, double value) const
{
  ConstIterator i = params.find(name);
  if ((i != params.end()) && (entry(i).isDouble()))
    return (entry(i).getDoubleValue() == value);
  return false;
}

bool APPSPACK::Parameter::List::isParameterEqual(const string& name, const char* value) const
{
  ConstIterator i = params.find(name);
  if ((i != params.end()) && (entry(i).isString()))
    return (entry(i).getStringValue() == value);
  return false;
}

bool APPSPACK::Parameter::List::isParameterEqual(const string& name, const string& value) const
{
  ConstIterator i = params.find(name);
  if ((i != params.end()) && (entry(i).isString()))
    return (entry(i).getStringValue() == value);
  return false;
}

bool APPSPACK::Parameter::List::isParameterEqual(const string& name, const Vector& value) const
{
  ConstIterator i = params.find(name);
  if ((i != params.end()) && (entry(i).isVector()))
    return (entry(i).getVectorValue() == value);
  return false;
}

APPSPACK::Parameter::List& APPSPACK::Parameter::List::sublist(const string& name)
{
  // Find name in list, if it exists.
  Iterator i = params.find(name);

  // If it does exist and is a list, return the list value.
  // Otherwise, throw an error.
  if (i != params.end()) {
    if (entry(i).isList()) 
      return (entry(i).getListValue());
    else
      cerr << "ERROR: Parameter " << name << " is not a list." << endl;
      throw "NOX Error";
  }

  // If it does not exist, create a new empty list and return a reference
  return params[name].setList(true);
}

const APPSPACK::Parameter::List& APPSPACK::Parameter::List::sublist(const string& name) const
{
  // Find name in list, if it exists.
  ConstIterator i = params.find(name);

  // If it does not exist, throw an error
  if (i == params.end()) {
    cerr << "ERROR: Parameter " << name << " is not a valid list." << endl;
    throw "NOX Error";
  }

  // If it does exist and is a list, return the list value.
  if (entry(i).isList()) 
    return (entry(i).getListValue());

  // Otherwise, the parameter exists but is not a list. Throw an error.
  cerr << "ERROR: Parameter " << name << " is not a list." << endl;
  throw "NOX Error";
}
  
ostream& APPSPACK::Parameter::List::print(ostream& stream, int indent) const
{
  if (params.begin() == params.end()) 
  {
    for (int j = 0; j < indent; j ++)
      stream << ' ';
    stream << "[empty list]" << endl;
  }
  else 
    for (ConstIterator i = params.begin(); i != params.end(); ++i) 
    {
      for (int j = 0; j < indent; j ++)
	stream << ' ';
      if (entry(i).isList()) 
      {
	stream << name(i) << " -> " << endl;
	entry(i).getListValue().print(stream, indent + 2);
      }
      else
	stream << name(i) << " = " << entry(i) << endl;
    }
  return stream;
}

void APPSPACK::Parameter::List::pack() const
{
  for (ConstIterator i = params.begin(); i != params.end(); ++i) 
  {
    GCI::pack(NEW_ENTRY);
    GCI::pack(name(i));
    entry(i).pack();
  }
  GCI::pack(END_OF_LIST);
}

void APPSPACK::Parameter::List::unpack()
{
  int code;
  string name;
  GCI::unpack(code);

  while (code != END_OF_LIST)
  {
    GCI::unpack(name);
    params[name].unpack();
    GCI::unpack(code);
  }

}


#ifdef SNL_TFLOPS_ENV

const string& APPSPACK::Parameter::List::name(ConstIterator i) const
{
  return ((*i).first);
}

APPSPACK::Parameter::Entry& APPSPACK::Parameter::List::entry(Iterator i)
{
  return ((*i).second);
}

const APPSPACK::Parameter::Entry& APPSPACK::Parameter::List::entry(ConstIterator i) const
{
  return ((*i).second);
}

#else

const string& APPSPACK::Parameter::List::name(ConstIterator i) const
{
  return (i->first);
}

APPSPACK::Parameter::Entry& APPSPACK::Parameter::List::entry(Iterator i)
{
  return (i->second);
}

const APPSPACK::Parameter::Entry& APPSPACK::Parameter::List::entry(ConstIterator i) const
{
  return (i->second);
}

#endif


