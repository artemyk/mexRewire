// =============================================================================
// == addMatrices.cpp
// == --------------------------------------------------------------------------
// == This is a tutorial file on how to use MEX (i.e. programming in C++ for
// ==   Matlab). This simple tutorial will show you how to add two double
// ==   precision matrices using C++, meaning this function is basically useless
// ==   besides being used as a learning example.
// ==
// == Note: I assume here that all inputs are 2D... I'm too lazy to do 3D
// == --------------------------------------------------------------------------
// == Written by Jason Chang 01-30-2011
// =============================================================================

// this is required for all MEX functions
#include "mex.h"
#include <iostream>

// this is my own helper file
#include "helperMEX.h"

// I like to write the Matlab usage here:
// [Z] = addMatrices(X, Y)

// Here, we define the number of inputs and outputs
#define NUMARGS 2
#define NUMOUT 1

// This is the declaration of the function... nothing here should really be changed
// nlhs is the Number of variables on the Left Hand Side of the equals sign from Matlab
// nlhs is the Number of variables on the Right Hand Side of the equals sign from Matlab
// plhs is the array of outputs (each one needs to be created and assigned here)
// prhs is the array of inputs
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
   // Check for proper number of arguments
   if (nrhs != NUMARGS)
      mexErrMsgTxt("Incorrect number of input arguments required.");
   else if (nlhs > NUMOUT)
      mexErrMsgTxt("Too many output arguments expected.");
   
   // check to make sure we have the right input types
   checkInput(prhs, 0, "matrix", "double");
   checkInput(prhs, 1, "matrix", "double");
   
   // get the size of the inputs
   int mrows = mxGetM(prhs[0]);
   int ncols = mxGetN(prhs[0]);
   
   // let's check to make sure the other input is of the same size
   if (mxGetM(prhs[1]) != mrows || mxGetN(prhs[1]) != ncols)
      mexErrMsgTxt("Inputs are of different size.");
   
   // Let's create and assign the inputs to local pointers for easier use
   double* X = (double *) mxGetData(prhs[0]);
   double* Y = (double *) mxGetData(prhs[1]);
   
   // We first have to create the space for the output matrix
   plhs[0] = mxCreateNumericMatrix(mrows,ncols, mxDOUBLE_CLASS, mxREAL);
   // Note: instead of using mxDOUBLE_CLASS, other things can be used.
   // Check helperMEX.h -  checkInput(...) for a list of some other classes
   
   // Now that the memory is created for the return argument, let's assign the
   // memory to a local pointer for easier use
   double* Z = (double *) mxGetData(plhs[0]);
   
   // Here, I would usually just call a function written in another *.cpp file
   // so that the function can be shared by Matlab and C++ programs. Because this
   // function is so simple, this seems silly, so I will just write it here.
   for (int i=0; i<mrows*ncols; i++)
      Z[i] = X[i] + Y[i];
   
   // Let's print out the matrix, just so you know how to print stuff
   mexPrintf("Here is the matrix!\n");
   for (int r=0; r<mrows; r++)
   {
      for (int c=0; c<ncols; c++)
         mexPrintf("%f\t", Z[findIndex(mrows, r,c)]);
      mexPrintf("\n");
   }
   
   /*
   // This is a faster way of printing
   mexPrintf("Here is the matrix!\n)";
   int index = 0;
   for (int r=0; r<mrows; r++)
   {
      for (int c=0; c<ncols; c++)
         mexPrintf("%f\t", Z[index++]);
      mexPrintf("\n");
   }
   */
   
   // That's it!  Compile with "mex -g addMatrices.cpp" in Matlab. Test it out with:
   // Z = addMatrices(rand(3), rand(3));
}
