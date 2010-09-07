
------------------------------------------

Custom Executor Example for APPSPACK

------------------------------------------

We provide an example of writing a custom APPSPACK::Executor object
and associated main() for APPSPACK. A custom executor allows the user
to employ a different parallel interface than what we provide. For
example, the executor could be customized to do multi-level
parallelism in which each function evaluation uses multiple
processors.

This directory contains a very simple example of a custom
APPSPACK::Executor object. In our custom executor, we use MPI directly
rather than the APPSPACK::GCI interface to MPI. Moreover, we link
directly to the function evaluation rather than doing an external
system call. The objective function is two dimensional with

	f(x) = sum i * x_i^2.
  
and the constraint

	sum x_i^2 <= 1

Each variable is bounded below by -1 and above by 3. The initial point
is x_0 = (.01,.3).

Here is a description of the files included in this directory:

Makefile - An example Makefile that will need to be customized to your
           environment. (See Notes 1-2.)

msgtags.hpp - Contains definitions of the message tags that are
              used in the MPI calls.

executor.hpp - Header files for the CustomExecutor object that derives
               from the APPSPACK::Executor::Interface. Contains
               comments about how the interface should be written.

executor.cpp - Source file for the CustomExecutor object, containing
               comments on how the interface should be written.


master.cpp - The file containing the master() function, which in turn
             creates the custom executor as well as calling the
             APPSPACK solver.

worker.cpp - Actually does the function evaluation.

main.cpp - The file containing main(). It initializes MPI and calls
           the worker and master tasks as appropriate.

custom.out - Sample output file; Your results may vary due to 
             the asynchronism in APPSPACK

Note 1: It's important to use the same compiler as was used in
compiling APPSPACK.

Note 2: It doesn't matter whether or not APPSPACK was compiled with
MPI enabled because that only affects the parallel parts of APPSPACK
which are replaced here with customized versions. 

Note 3: We do assume that --enable-cddlib was turned on during
configuration.

After compiling, the executable appspack-custom will appear in this
directory.  To run in MPI, the command line is (depend on which MPI
you are using)

  mpirun -np 5 appspack_custom 2

This custom version does *not* require a dot-apps (.apps) file to
specify the solver options. Instead, they are hard-wired into the  
code. It could easily read the inputs in from a dot-apps file using
APPSPACK::parseTextInputFile(), but we have opted for this 
implementation.

However, the number of parameters in the optimization problem is not 
hard-wired and must be specified in the command line.  (Hence, the 2
at the end of the example command line.)
