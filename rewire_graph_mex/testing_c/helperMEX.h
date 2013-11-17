// =============================================================================
// == helperMEX.h
// == --------------------------------------------------------------------------
// == A header file that includes some useful functions when programming in C++
// ==   for Matlab (using MEX).
// == --------------------------------------------------------------------------
// == Written by Jason Chang 01-30-2011
// =============================================================================
#include "mex.h"
#include <cstring>

// -----------------------------------------------------------------------------
// -- checkInput
// --   Checks one element in the input array (prhs[index]). Makes sure that the
// -- input has the right dimensions (scalar, vector, or matrix) and is of the
// -- right type (logical, single, double, char, (u)int8, (u)int16, (u)int32,
// -- (u)int64, cell).
// --
// --   parameters:
// --     - prhs : the array of arguments passed into the MEX function
// --     - index : the index into prhs of the argument to check
// --     - dimensions : a string indicating what the argument should be. If ""
// --         is used (i.e. an empty string), then this is not checked.
// --         Otherwise, this should be "scalar", "vector", or "matrix".
// --     - type : a string indicating what type of variable the argument should
// --         be. This is extremely important to prevent segmentation faults.
// --         Possible strings are "logical", "single", "double", "char",
// --         "uint8", "int8", "uint16", "int16", "uint32", "int32", "uint64",
// --         "int64", and "cell"
// -----------------------------------------------------------------------------
inline void checkInput(const mxArray *prhs[], int index, char* dimensions, char* type)
{
   int mrows = mxGetM(prhs[index]);
   int ncols = mxGetN(prhs[index]);
      
   if (strcmp(dimensions, "scalar")==0)
   {      
      if (!(mrows==1 && ncols==1))
      {
         mexPrintf("\n--> Input Number: %d\n", index);
         mexErrMsgTxt("    Should be a scalar");
      }
   }
   else if (strcmp(dimensions, "vector")==0)
   {
      if ((mrows==1 && ncols==1) || (mrows!=1 && ncols!=1))
      {
         mexPrintf("\n--> Input Number: %d\n", index);
         mexErrMsgTxt("    Should be a vector");
      }
   }
   else if (strcmp(dimensions, "matrix")==0)
   {
      if (mrows==1 || ncols==1)
      {
         mexPrintf("\n--> Input Number: %d\n", index);
         mexErrMsgTxt("    Should be a matrix");
      }
   }
   else if (strcmp(dimensions, "")!=0)
   {
      mexPrintf("\n--> Input Number: %d\n", index);
      mexErrMsgTxt("    Check dimensions is wrong.  Possible values are \"scalar\", \"vector\", \"matrix\", or \"\" (no check).");
   }
   
   if (strcmp(type, "logical")==0)
   {
      if (mxGetClassID(prhs[index])!=mxLOGICAL_CLASS)
      {
         mexPrintf("\n--> Input Number: %d\n", index);
         mexErrMsgTxt("    Should be logical.");
      }
   }
   else if (strcmp(type, "single")==0)
   {
      if (mxGetClassID(prhs[index])!=mxSINGLE_CLASS)
      {
         mexPrintf("\n--> Input Number: %d\n", index);
         mexErrMsgTxt("    Should be single.");
      }
   }
   else if (strcmp(type, "double")==0)
   {
      if (mxGetClassID(prhs[index])!=mxDOUBLE_CLASS)
      {
         mexPrintf("\n--> Input Number: %d\n", index);
         mexErrMsgTxt("    Should be double.");
      }
   }
   else if (strcmp(type, "int8")==0)
   {
      if (mxGetClassID(prhs[index])!=mxINT8_CLASS)
      {
         mexPrintf("\n--> Input Number: %d\n", index);
         mexErrMsgTxt("    Should be int8.");
      }
   }
   else if (strcmp(type, "uint8")==0)
   {
      if (mxGetClassID(prhs[index])!=mxUINT8_CLASS)
      {
         mexPrintf("\n--> Input Number: %d\n", index);
         mexErrMsgTxt("    Should be uint8.");
      }
   }
   else if (strcmp(type, "int16")==0)
   {
      if (mxGetClassID(prhs[index])!=mxINT16_CLASS)
      {
         mexPrintf("\n--> Input Number: %d\n", index);
         mexErrMsgTxt("    Should be int16.");
      }
   }
   else if (strcmp(type, "uint16")==0)
   {
      if (mxGetClassID(prhs[index])!=mxUINT16_CLASS)
      {
         mexPrintf("\n--> Input Number: %d\n", index);
         mexErrMsgTxt("    Should be uint16.");
      }
   }
   else if (strcmp(type, "int32")==0)
   {
      if (mxGetClassID(prhs[index])!=mxINT32_CLASS)
      {
         mexPrintf("\n--> Input Number: %d\n", index);
         mexErrMsgTxt("    Should be int32.");
      }
   }
   else if (strcmp(type, "uint32")==0)
   {
      if (mxGetClassID(prhs[index])!=mxUINT32_CLASS)
      {
         mexPrintf("\n--> Input Number: %d\n", index);
         mexErrMsgTxt("    Should be uint32.");
      }
   }
   else if (strcmp(type, "int64")==0)
   {
      if (mxGetClassID(prhs[index])!=mxINT64_CLASS)
      {
         mexPrintf("\n--> Input Number: %d\n", index);
         mexErrMsgTxt("    Should be int64.");
      }
   }
   else if (strcmp(type, "uint64")==0)
   {
      if (mxGetClassID(prhs[index])!=mxUINT64_CLASS)
      {
         mexPrintf("\n--> Input Number: %d\n", index);
         mexErrMsgTxt("    Should be uint64.");
      }
   }
   else if (strcmp(type, "char")==0)
   {
      if (mxGetClassID(prhs[index])!=mxCHAR_CLASS)
      {
         mexPrintf("\n--> Input Number: %d\n", index);
         mexErrMsgTxt("    Should be char.");
      }
   }
   else if (strcmp(type, "cell")==0)
   {
      if (mxGetClassID(prhs[index])!=mxUINT64_CLASS)
      {
         mexPrintf("\n--> Input Number: %d\n", index);
         mexErrMsgTxt("    Should be cell.");
      }
   }
}

// -----------------------------------------------------------------------------
// -- findCoordinates
// --   Converts an index in a 1D array to 2D coordinates.
// --
// --   parameters:
// --     - numRows : the number of rows in the matrix to index into
// --     - index : the index to convert from
// --   return parameters:
// --     - (x,y) : the coordinates of index mapped into the matrix
// -----------------------------------------------------------------------------
inline void findCoordinates(int numRows, int index, int &x, int &y)
{
   x = index%numRows;
   y = index/numRows;
}

// -----------------------------------------------------------------------------
// -- findIndex
// --   Converts from 2D coordinates to a 1D index
// --
// --   parameters:
// --     - numRows : the number of rows in the matrix to index into
// --     - (x,y) : the coordinates to convert from
// -- 
// --   return value:
// --     - Returns the index converted from the coordinates
// -----------------------------------------------------------------------------
inline int findIndex(int numRows, int x, int y)
{
   return y*numRows + x;
}
// -----------------------------------------------------------------------------
// -- findIndex
// --   Converts from 3D coordinates to a 1D index
// --
// --   parameters:
// --     - numCols : the number of columns in the matrix to index into
// --     - numRows : the number of rows in the matrix to index into
// --     - (x,y,z) : the coordinates to convert from
// -- 
// --   return value:
// --     - Returns the index converted from the coordinates
// -----------------------------------------------------------------------------
inline int findIndex(int numRows, int numCols, int x, int y, int z)
{
   return z*numRows*numCols + y*numRows + x;
}
