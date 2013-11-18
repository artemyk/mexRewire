% mex -g -O rewire_graph_cpp.cpp CXXFLAGS="\$CXXFLAGS -fopenmp -O3 -DVECLIB" LDFLAGS="\$LDFLAGS -fopenmp -framework Accelerate"
mex -g -O rewire_graph_cpp.cpp CXXFLAGS="-O3"

