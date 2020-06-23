img = imread("lena.png");   % Read image
gray_img = rgb2gray(img);   % Convert image to gray
flat_img = double(gray_img(:)); % Data type conversion for std calculation
avg = mean(flat_img);
sd = std(flat_img);
[max_value, max_index] = max(flat_img);
[min_value, min_index] = min(flat_img);

[max_loc_x, max_loc_y] = ind2sub(size(gray_img),max_index); % Index to subscript
[min_loc_x, min_loc_y] = ind2sub(size(gray_img),min_index); % Index to subscript