
------------------------------------------

Custom Evaluator Example for APPSPACK

------------------------------------------

We provide an example a custom APPSPACK::Evaluator object and associated 
main() for APPSPACK. A custom evaluator allows the user to link 
APPSPACK directly to the function evaluation and eliminates the  
an external system calls which require a system of file input and output. 

This directory contains a very simple example of a custom
APPSPACK::Evaluator object. The objective function is two dimensional with

	f(x) = sum i * x_i^2.
  
and the constraint

	sum x_i^2 >= 1

Each variable is bounded below by -1 and above by 3. The initial point
is x_0 = (.01,.3).

Here is a description of the files.

Makefile - An example Makefile that will need to be customized to your

cev_evaluator.hpp - Header files for the CustomEvaluator object that
           derives from the APPSPACK::Evaluator::Interface. Contains
           comments about how the interface should be written.

cev_evaluator.cpp - Source file for the CustomEvaluator object,
           containing comments on how the interface should be written.

cev_main.cpp - The file containing main(). It initializes MPI and has
           the short routines for the master and worker. The custom
           evaluator is used by the worker.

cev_custom.out - Sample output file; Your results may vary due to the
           asynchronism in APPSPACK

Note 1: It's important to use the same compiler as was used in
compiling APPSPACK.

Note 2: APPSPACK must be configured and compiled with both
--with-mpi-compilers and --enable-cddlib options for the example to
work.

After compiling, the executable appspack-custom will appear in this
directory.  To run in MPI, the command line will depend on which MPI
you are using. In MPICH, the command looks like

  mpirun -np 5 appspack_cev

This custom version does *not* require a dot-apps (.apps) file to
specify the solver options. Instead, they are hard-wired into the  
code. It could easily read the inputs in from a dot-apps file using
APPSPACK::parseTextInputFile(), but we have opted for this 
implementation to show that there are different ways to create the 
parameter list that is needed by the Bounds and Solver objects.



