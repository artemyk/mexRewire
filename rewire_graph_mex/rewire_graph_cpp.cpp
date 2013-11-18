// Matrix rewiring code, adapted from "sym_generate_srand"
// Artemy Kolchinsky, akolchin@indiana.edu


#include "mex.h"
#include <omp.h>
#include <iostream>
#include <vector>
#include <map>
#include <math.h>

#define NUMARGS 2
#define NUMOUT 1

inline double unifRand() {
    return std::rand() / double(RAND_MAX);
}

// GRAPH, # rewires, # return graphs
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    // Check for proper number of arguments
    // if (nrhs != NUMARGS) mexErrMsgTxt("Incorrect number of input arguments required.");
    if (nlhs > NUMOUT) mexErrMsgTxt("Too many output arguments expected.");
    
    // Check data type of input argument.
    if (!(mxIsDouble(prhs[0]))) mexErrMsgTxt("Input array must be of type double.");
    if (!(mxIsDouble(prhs[1]))) mexErrMsgTxt("2nd Input argument must be of type double.");
    // if (!(mxIsDouble(prhs[2]))) mexErrMsgTxt("3rd Input argument must be of type double.");
    
    
    const mwSize *dim_array;
    dim_array = mxGetDimensions(prhs[0]);
    int N = dim_array[0];
    int N2= dim_array[1];
    
    if (N != N2)  mexErrMsgTxt("Matrix not square!");
    
    const double * graph = (double *) mxGetData(prhs[0]);
    
    const unsigned int NUM_RETURN_DIMS = 2;
    mwSize * out_dims = (mwSize *) mxMalloc (NUM_RETURN_DIMS * sizeof(mwSize));
    out_dims[0] = N;
    out_dims[1] = N;
    
    plhs[0] = mxCreateNumericArray(NUM_RETURN_DIMS, out_dims, mxDOUBLE_CLASS, mxREAL);
    double * outGraph     = (double *) mxGetData(plhs[0]);
    
    
    std::vector<unsigned int> i_srand, j_srand;
    for (unsigned int i=0; i<N; i++) {
        for (unsigned int j=0; j<i; j++) {
            if (i <= j) continue;
            double cWeight = graph[i*N+j];
            if (cWeight != 0.0) {
                i_srand.push_back(i);
                j_srand.push_back(j);
                outGraph[N*i+j] = cWeight;
                outGraph[N*j+i] = cWeight;
            }
        }
    }
    unsigned int Ne = i_srand.size();
    
    double * constraintGraph = NULL;
    if (nrhs > 2) {
        // Some constraint graph, only edges preserving this can be swapped
        // For example, for distance sensitive rewiring, this argument is distance matrix
        constraintGraph  = (double *) mxGetData(prhs[2]);
        const mwSize * dim_array_constraint;
        dim_array_constraint = mxGetDimensions(prhs[2]);
        if (dim_array_constraint[0] != N || dim_array_constraint[1] != N) {
            mexErrMsgTxt("Constraint matrix should be same size as input graph matrix");
        }
            
    }
    if (nrhs > 3) {
        mexErrMsgTxt("Incorrect number of input arguments required.");
    }
        

    unsigned long numRewiresPerEdge =  static_cast<unsigned long>(*((double *) mxGetData(prhs[1])));
    unsigned long constraintSkipped = 0;
    if (numRewiresPerEdge == 0) numRewiresPerEdge = 4;
    unsigned long numRewires = Ne * numRewiresPerEdge;
    // std::cout << "# of vertices: " << N << "\t# of edges/2: " << Ne << " / #rewires: "<<  numRewires << " / # graphs: " << returnGraphs << std::endl;
    
    unsigned long MAX_ITERS = 10000000, num_attempts = 0, actualRewires = 0;
    for (; num_attempts < MAX_ITERS && actualRewires < numRewires; num_attempts++) {
        unsigned int e1=floor(Ne*unifRand());
        unsigned int e2=floor(Ne*unifRand());
        unsigned int v1=i_srand[e1], v2=j_srand[e1];
        unsigned int v3=i_srand[e2], v4=j_srand[e2];
        if (v1!=v3 && v1!=v4 && v2!=v4 && v2!=v3) {
            if (unifRand()>0.5) {
                unsigned int v5=v3; v3=v4; v4=v5;
            }
            if (constraintGraph != NULL) {
                if (constraintGraph[N*v1+v2] != constraintGraph[N*v1+v3] || constraintGraph[N*v3+v4] != constraintGraph[N*v4+v2]) {
                    constraintSkipped += 1;
                    continue; // constraints don't match
                }
            }
            if (outGraph[v1*N+v3]==0 && outGraph[v2*N+v4]==0){
                double w12 = outGraph[N*v1+v2];
                double w34 = outGraph[N*v4+v3];
                outGraph[N*v1+v2]=0;
                outGraph[N*v3+v4]=0;
                outGraph[N*v2+v1]=0;
                outGraph[N*v4+v3]=0;
                outGraph[N*v1+v3]=w12;
                outGraph[N*v2+v4]=w34;
                outGraph[N*v3+v1]=w12;
                outGraph[N*v4+v2]=w34;
                i_srand[e1]=v1;
                j_srand[e1]=v3;
                i_srand[e2]=v2;
                j_srand[e2]=v4;
                actualRewires++;
            }
        }
    }
    
    if (num_attempts == MAX_ITERS) {
      std::cout << actualRewires << " / " << numRewires << " rewires done (" << constraintSkipped << " skipped due to constraints), but..." << std::endl;       

      mexErrMsgTxt("MAX_ITERS reached!");
    }

}
