addpath('rewire_graph_mex');

load('sample_mxs', 'sc_mx','dist_mx');

Ne = sum(sc_mx(:));
fprintf('\nNetwork of size N=%d, density=%0.3f\n\n', size(sc_mx,1), mean(sc_mx(:)));


% sc_mx is a sample structural adjacency matrix that we will rewire

% dist_mx is a sample distance matrix that will serve as a constraint
% matrix to the rewiring code.  

% The constraint matrix can be passed in as the optional third parameter to
% the rewiring function rewire_graph_cpp.  Then, the rewiring  code will
% only rewire an edge from i->j to k->l if and only if 
%
%  constraint_matrix(i,j) = constraint_matrix(k,l)
%
% So, for example, if constraint_matrix(i,j) contains the distance from
% i to j, it means edges will be rewired in a distance-preserving manner.

% Notice, however, that if the constraint matrix contains a continuous 
% range of values, it may be impossible to find pairs (i,j) and (k,l) that
% both have the exact same values in the constraint matrix.  For this 
% reason, we can first 'quantize' the constraint matrix to contain only
% one of several discrete values.


% Quantile distance matrix to one of QUANTIZE_LEVELS levels
QUANTIZE_LEVELS = 10;
dist_mx_quantized = zeros(size(dist_mx));
for i = 0:QUANTIZE_LEVELS-1
    dist_mx_quantized( dist_mx(:) > quantile(dist_mx(:), i/QUANTIZE_LEVELS) ) = i + 1;
end

% original graph rewired with BCT function
rw1 = randmio_und(sc_mx, 4);      

% original graph rewired w. rewire_graph_cpp
rw2 = rewire_graph_cpp(sc_mx, 4); 
if any(sum(sc_mx,1) ~= sum(rw2,1)) % sanity check
    error('Degrees not preserved by rewire_graph_cpp!');
end

% original graph rewired w. rewire_graph_cpp and a constraint matrix
rw3 = rewire_graph_cpp(sc_mx, 4, dist_mx_quantized);
if any(sum(sc_mx,1) ~= sum(rw3,1)) % sanity check
    error('Degrees not preserved by rewire_graph_cpp w. constraints!');
end

%% PLOT THE RESULTS

figure;
subplot(2,4,1);
spy(sc_mx);
title('Original Graph');
subplot(2,4,2);
spy(rw1);
title('Rewired BCT');
subplot(2,4,3);
spy(rw2);
title('rewire\_graph\_cpp');
subplot(2,4,4);
spy(rw3);
title('rewire\_graph\_cpp w. constraint');

subplot(2,4,5);
hist(dist_mx_quantized(sc_mx==1));
title('Contraint hist (orig)');
subplot(2,4,6);
hist(dist_mx_quantized(rw1==1));
title('Contraint hist (BCT rw)');
subplot(2,4,7);
hist(dist_mx_quantized(rw2==1));
title('Contraint hist (rewire\_graph\_cpp)');
subplot(2,4,8);
hist(dist_mx_quantized(rw3==1));
title('Contraint hist (rewire\_graph\_cpp w/ constraint)');

%%

ITERS = 10;
fprintf('*** BENCHMARKING: running %d iterations ***\n', ITERS);

fprintf('randmio_und       : ');
tic();
for i=1:ITERS, randmio_und(sc_mx, 4); end
toc();

%We can also compare it to Maslov's own sym_generate_srand, available
%from http://www.cmth.bnl.gov/~maslov/sym_generate_srand.m .
%{
fprintf('sym_generate_srand: ');
tic();
for i=1:ITERS, sym_generate_srand(G, 4*Ne); end
toc();
%}

% Sometimes first time mex file is called its slower, so make sure we
% get rid of this effect by calling it once:
rewire_graph_cpp(sc_mx, 4);

% Now run the benchmark
fprintf('rewire_graph_cpp  : ');
tic();
for i=1:ITERS, rewire_graph_cpp(sc_mx, 4); end
toc();



