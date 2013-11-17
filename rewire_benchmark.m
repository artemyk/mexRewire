N = 200;
G = randn(N);
G = (G+G') ;
G(eye(N)==1) = min(G(:));
G = double( G > quantile(G(:), 0.9) );
Ne = sum(G(:));
fprintf('\nNetwork of size N=%d, density=%0.3f\n\n', N, mean(G(:)));

ITERS = 50;
fprintf('randmio_und       : ');
tic();
for i=1:ITERS, randmio_und(G, 4); end
toc();

%fprintf('sym_generate_srand: ');
%tic();
%for i=1:ITERS, sym_generate_srand(G, 4*Ne); end
%toc();


addpath('rewire_graph_mex');
fprintf('rewire_graph_cpp  : ');
tic();
for i=1:ITERS
  rewire_graph_cpp(G, 4);
end
toc();



