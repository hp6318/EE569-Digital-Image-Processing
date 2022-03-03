%EE559 ---  HW2_Q1c ---  Hardik Prajapati --- 2678294168 
%Windows --- matlab2017b 
%Project: Structured Edge detection
%Arguments to be passed: 0
%input image file location and output image file locations are hard-coded. 
%@credits: open_source code provided by the TA 


% Demo for Structured Edge Detector (please see readme.txt first).

%% set opts for training (see edgesTrain.m)
opts=edgesTrain();                % default options (good settings)
opts.modelDir='models/';          % model will be in models/forest
opts.modelFnm='modelBsds';        % model name
opts.nPos=5e5; opts.nNeg=5e5;     % decrease to speedup training
opts.useParfor=0;                 % parallelize if sufficient memory

%% train edge detector (~20m/8Gb per tree, proportional to nPos/nNeg)
tic, model=edgesTrain(opts); toc; % will load model if already trained

%% set detection parameters (can set after training)
model.opts.multiscale=0;          % for top accuracy set multiscale=1
model.opts.sharpen=2;             % for top speed set sharpen=0
model.opts.nTreesEval=4;          % for top speed set nTreesEval=1
model.opts.nThreads=4;            % max number threads for evaluation
model.opts.nms=0;                 % set to true to enable nms

%% evaluate edge detector on BSDS500 (see edgesEval.m)
if(0), edgesEval( model, 'show',1, 'name','' ); end

%% detect edge and visualize results
I = imread('HW2_images/Pig.jpg');
tic, E=edgesDetect(I,model); toc
figure(1); im(I); figure(2); im(1-E);

%% generate edge map: Probability>threshold = 0 (edge) else 255(background)
%%E(1,1)
edge_map=zeros(321,481);
thresh=0.15;
for i=1:321
    for j=1:481
        if E(i,j)>thresh
            edge_map(i,j)=0;
        else
            edge_map(i,j)=255;
        end
    end
end
figure(3); im(edge_map);

%%save probability edge image
imwrite(E,'C:\Users\hp631\OneDrive\Documents\MATLAB\USC\HW2_results_matlab\pig_se_probability.jpg');

%%save edge map image
imwrite(edge_map,'C:\Users\hp631\OneDrive\Documents\MATLAB\USC\HW2_results_matlab\pig_se_0point15.jpg');
