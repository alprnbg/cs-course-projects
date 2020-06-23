clear; close all; clc;

% Read the joker image. 
img = im2double(imread("jokerimage.jpg"));
figure;
imshow(img);
title("Original Image");

% Blurring Kernel
figure;
blur_kernel = [0.0625 0.1250 0.0625;
               0.1250 0.2500 0.1250;
               0.0625 0.1250 0.0625];
blur_out = Conv2D(img, blur_kernel);
imshow(blur_out);
title("Blurred Image");

% Sharpening Kernel
figure;
sharpen_kernel = [ 0 -1  0;
                  -1  5 -1;
                   0 -1  0];
sharpen_out = Conv2D(img, sharpen_kernel);
imshow(sharpen_out);
title("Sharpened Image");

% Edge Highlighting Kernel (Sobel)
figure;
edge_kernel_x = [+1 +2 +1; 
                  0  0  0;
                 -1 -2 -1];
edge_kernel_y = edge_kernel_x';
edge_out_x = Conv2D(img, edge_kernel_x);
edge_out_y = Conv2D(img, edge_kernel_y);
edge_out = sqrt(edge_out_x.^2 + edge_out_y.^2);
imshow(edge_out);
title("Image with Highlighted Edges");

% Emboss Kernel
figure;
embose_kernel = [ -2 -1  0;
                  -1  1  1;
                   0  1  2];
embose_out = Conv2D(img, embose_kernel);
imshow(embose_out);
title("Embossed Image");

