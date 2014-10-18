mexRewire
=========

C++ code for degree-preserving matrix rewiring (mex-wrapped for use in Matlab)


Installation
------------
Download the code.  Then, to build the mex file, change to the `rewire_graph_mex` directory and in MATLAB run:

```
make
```

Also make sure that the `rewire_graph_mex` directory is in your MATLAB path.

Usage
-----

This mex module provides a function to do degree-preserving rewiring a graph.  It can be called by:

```
rewiredGraph = rewire_graph_cpp(G, num_of_rewires_per_edge [, constraintMatrix]);
```

Here, `G` is the graph to rewire, and `num_of_rewires_per_edge` is how many rewires per edge to carry out.

A matrix can be passed into the optional third parameter `constraintMatrix` if certain constraints should hold while rewiring.  Specifically, if `constraintMatrix` is passed in, the rewiring  code will only rewire an edge from i->j to k->l if and only if `constraint_matrix(i,j) = constraint_matrix(k,l)`. So, for example, if `constraint_matrix(i,j)` contains the distance from i to j, edges will be rewired in a distance-preserving manner.

Notice that if the constraint matrix contains a continuous range of values, it may be impossible to find pairs (i,j) and (k,l) that both have the exact same values in the constraint matrix. In this case, the constraint can be 'softened' by first quantizing the constraint matrix to contain only one of several discrete values.

See `rewire_example.m` for a demonstration of use.
