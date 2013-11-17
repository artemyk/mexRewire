/*
 * =============================================================
 * findnz.c 
 * Example for illustrating how to handle N-dimensional arrays in 
 * a MEX-file. NOTE: MATLAB uses 1-based indexing, C uses 0-based 
 * indexing.
 *
 * Takes an N-dimensional array of doubles and returns the indices 
 * for the non-zero elements in the array. findnz works 
 * differently than the FIND command in MATLAB in that it returns 
 * all the indices in one output variable, where the column 
 * element contains the index for that dimension.
 *  
 *
 * This is a MEX-file for MATLAB.  
 * Copyright (c) 1984-2000 by The MathWorks, Inc.
 * =============================================================
 */

/* $Revision: 1.9 $ */

#include "mex.h"
#include <cmath>
#include <omp.h>

// #include <stdio.h>

#define NUMARGS 3
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
   

  /* Check data type of input argument. */
  if (!(mxIsSingle(prhs[0]))) {
    mexErrMsgTxt("Input array must be of type single.");
  }
  if (!(mxIsSingle(prhs[1]))) {
    mexErrMsgTxt("Input array must be of type single.");
  }
   
  const mwSize *dim_array;  
  dim_array = mxGetDimensions(prhs[0]);
  unsigned num_trials = dim_array[0];
  unsigned num_chs    = dim_array[1];
  unsigned num_samples= dim_array[2];
  float f_num_samples(num_samples);
  
  float * leftData  = (float *) mxGetData(prhs[0]);
  float * rightData = (float *) mxGetData(prhs[1]);
  float * windowFunction = (float *) mxGetData(prhs[2]);
  
  unsigned windowSize = mxGetNumberOfElements(prhs[2]);
  bool doWindowing = false;
  if (windowSize > 0) {
      if (windowSize != num_samples) { mexErrMsgTxt("Window size must be same as # of samples"); }
      if (!(mxIsSingle(prhs[2])))    { mexErrMsgTxt("Window must be of type single.");           }
      doWindowing = true;
  }
  mwSize * out_dims = (mwSize *) mxMalloc (3 * sizeof(mwSize));
  out_dims[0] = num_trials;
  out_dims[1] = num_chs;
  out_dims[2] = num_chs;
  
  plhs[0] = mxCreateNumericArray(3, out_dims, mxSINGLE_CLASS, mxCOMPLEX);
  int mx1=mxGetNumberOfElements(plhs[0]);
  
  float * outData     = (float *) mxGetData(plhs[0]);
  float * outDataImag = (float *) mxGetImagData(plhs[0]);
  
  #pragma omp parallel for 
  // shared(leftData, rightData, outData, outDataImag) 
  for (unsigned cTrial = 0; cTrial < num_trials; cTrial++) {
      for (unsigned ch1 = 0; ch1 < num_chs ; ch1++) {
          for (unsigned ch2 = 0; ch2 < num_chs ; ch2++) {
              float phaseDiffReal = 0;
              float phaseDiffImag = 0; 
              for (unsigned tpoint = 0; tpoint < num_samples; tpoint++) {
                  float diff = leftData[ cTrial + ch1 * num_trials + tpoint * num_trials * num_chs ] 
                             - rightData[ cTrial + ch2 * num_trials + tpoint * num_trials * num_chs ];
                  // printf("%0.4f / %0.4f\n", leftData[ cTrial + ch1 * num_trials + tpoint * num_trials * num_chs ] , 
                  //        rightData[ cTrial + ch2 * num_trials + tpoint * num_trials * num_chs ]);
                  float mult = (doWindowing ? windowFunction[tpoint] : 1);
                  phaseDiffReal += mult * cos(diff) / f_num_samples;
                  phaseDiffImag += mult * sin(diff) / f_num_samples;
                  
              }
              // printf("%d\n",cTrial * num_trials + ch1 * num_chs + ch2 * num_chs);
              unsigned  long outNdx = cTrial + ch1 * num_trials + ch2 * num_trials * num_chs;
              outData[     outNdx ] = phaseDiffReal;
              outDataImag[ outNdx ] = phaseDiffImag;
              // printf("%0.10f / %0.10f\n", phaseDiffReal, phaseDiffImag);
              // return;
          }
      }
  }
}