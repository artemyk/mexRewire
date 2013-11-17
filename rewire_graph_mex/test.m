%%
addpath(genpath('../../code/'));
d=load('/Users/artemy/Google Drive/code/../dataNew/curated_data/griffa_scale_1_SC_indiv.mat');
mx = squeeze(d.SClen(:,:,1));
%binMx = zerodiags(double(sign(mx)));
tic();
G2=rewire_graph_cpp(binMx, 0); % , 100);
toc();
tic();
%sym_generate_srand(binMx)
toc();
