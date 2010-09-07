// $Id: cev_evaluator.hpp,v 1.3 2006/06/30 19:14:31 tgkolda Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/example-custom-evaluator/cev_evaluator.hpp,v $ 

/*! \file cev_evaluator.hpp 

\brief Header file for example custom evaluator 
\sa \ref pageCustomize_evaluator
*/


#include "APPSPACK_Evaluator_Interface.hpp"

//! Example custom evaluator derived from APPSPACK::Evaluator::Interface.
/*!    
  \sa \ref pageCustomize_evaluator
 */
class CustomEvaluator : public APPSPACK::Evaluator::Interface
{

public:

  /*! \brief Constructor - can take any arguments that are appropriate. In
    this case, we don't have any. */
  CustomEvaluator();

  //! Destructor
  ~CustomEvaluator();

  /*! 
  \brief Main function of the class because it does the evaluation.

   Evaluates the function at x, returning a vector of function values
   (usually just a single value) and any error messages. It must fill
   in the vector f_out and the string msg_out. For successful
   evaluations, msg_out should be something like "Success". If the
   iteration is unsuccessful, the vector f must be empty, i.e., by
   calling f_out.resize(0).

   See APPSPACK::Evaluator::Interface::operator()() for more details.
  */
  void operator()(int tag_in, const APPSPACK::Vector& x_in, 
		  APPSPACK::Vector& f_out, string& msg_out);

  //! Prints out information about the evaluator
  void print() const;

private:
  
  //! Return true is the constraint is satisfied, false otherwise.
  bool constraint(const APPSPACK::Vector& x);

  //! Evaluate function at x
  double feval(const APPSPACK::Vector& x);

  //! Number of calls to the evaluator
  int cnt;

};
