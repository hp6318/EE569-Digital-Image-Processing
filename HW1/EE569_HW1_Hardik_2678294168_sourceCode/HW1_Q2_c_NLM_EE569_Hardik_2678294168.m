clear all;
close all;
clc;

function writeraw(output_path, image_data)
    fid=fopen(output_path,'w+');
    fwrite(fid, image_data,'uint8');
    fclose(fid);
end


function readraw(input_path, height, width, channels)
    fin = fopen(input_path,'r');
    
    I = fread(fin, width*height*channels,'uint8=>uint8');
    I = reshape(I, [width height channels]);

    image_data = flip(imrotate(I, -90),2);
    
    fclose(fin);
end
flower_noisy = "E:\\usc\\Spring'22\\EE569 Image processing\\HW1\\Flower_gray_noisy.raw"; 
output = "E:\\usc\\Spring'22\\EE569 Image processing\\HW1\\results\\Flower_nlm.raw"; 
image_data = readraw(flower_noisy, 512, 768, 1);


DoS = 11;
SwS = 17;
CwS = 7;

[filteredImage, estDoS] = imnlmfilt(image_data,'DegreeOfSmoothing',DoS,'SearchWindowSize',SwS,'ComparisonWindowSize',CwS);

writeraw(output, filteredImage);