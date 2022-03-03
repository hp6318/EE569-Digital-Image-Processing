clear all;
close all;
clc;

flower_noisy = "E:\\usc\\Spring'22\\EE569 Image processing\\HW1\\Flower_gray_noisy.raw"; 
output = "E:\\usc\\Spring'22\\EE569 Image processing\\HW1\\results\\Flower_nlm.raw"; 
image_data = readraw(flower_noisy, 512, 768, 1);


DoS = 11;
SwS = 17;
CwS = 7;

[filteredImage, estDoS] = imnlmfilt(image_data,'DegreeOfSmoothing',DoS,'SearchWindowSize',SwS,'ComparisonWindowSize',CwS);

writeraw(output, filteredImage);