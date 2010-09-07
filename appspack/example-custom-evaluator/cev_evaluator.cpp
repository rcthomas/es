// $Id: cev_evaluator.cpp,v 1.3 2006/06/30 19:14:31 tgkolda Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/example-custom-evaluator/cev_evaluator.cpp,v $ 

#include "cev_evaluator.hpp"

/*! \file cev_evaluator.cpp 

\brief C++ file for example custom evaluator 
\sa \ref pageCustomize_evaluator

*/

CustomEvaluator::CustomEvaluator(): cnt(0)
{
}

CustomEvaluator::~CustomEvaluator()
{
  print();
}

void CustomEvaluator::operator()(int tag_in, const APPSPACK::Vector& x_in, 
				 APPSPACK::Vector& f_out, string& msg_out)
{
  if (constraint(x_in))
  {
    f_out.resize(1);		// Only a single function value
    f_out[0] = feval(x_in);	// Record the single function value
    msg_out = "Success";
  }
  else
  {
    f_out.resize(0);		// Empty function value
    msg_out = "Constraint Violation";
  }
  cnt ++;
  return;
}

void CustomEvaluator::print() const
{
  cout << "Number of evaluations is: " << cnt << endl;
}

double CustomEvaluator::feval(const APPSPACK::Vector& x)
{
  double f = 0;

  for (int i = 0; i < x.size(); i ++)
    f += (i + 1) * x[i] * x[i];

  return(f);
} 

bool CustomEvaluator::constraint(const APPSPACK::Vector& x)
{
  double tmp = 0;

  for (int i = 0; i < x.size(); i ++)
    tmp += x[i] * x[i];

  return (tmp >= 1.0);
} 
