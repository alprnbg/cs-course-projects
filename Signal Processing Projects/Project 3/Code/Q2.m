clear; close all; clc;

% Read the joker image. 
img = rgb2gray(imread("jokerimage.jpg"));
figure;
imshow(img);
title("Original Image");

% Read the object that will be detected. 
object = rgb2gray(imread("object.jpg"));
figure;
imshow(object);
title("Object");

% Extract Feature Points
object_points = detectSURFFeatures(object);
scene_points = detectSURFFeatures(img);

% Object Feature Points
figure;
imshow(object);
title('100 Strongest Feature Points from Box Image');
hold on;
plot(selectStrongest(object_points, 100));

% Scene Feature Points
figure;
imshow(img);
title('300 Strongest Feature Points from Scene Image');
hold on;
plot(selectStrongest(scene_points, 300));


% Feature Point Matching
[object_features, object_points] = extractFeatures(object, object_points);
[scene_features, scene_points] = extractFeatures(img, scene_points);

pairs = matchFeatures(object_features, scene_features);

matched_object_points = object_points(pairs(:, 1), :);
matched_scene_points = scene_points(pairs(:, 2), :);

figure;
showMatchedFeatures(object, img, matched_object_points, ...
    matched_scene_points, 'montage');
title('Putatively Matched Points (Including Outliers)');

[tform, inlier_box_points, inlier_scene_points] = ...
    estimateGeometricTransform(matched_object_points, matched_scene_points, 'affine');

figure;
showMatchedFeatures(object, img, inlier_box_points, ...
    inlier_scene_points, 'montage');
title('Matched Points (Inliers Only)');

box_polygon = [1, 1;...                        % top-left
        size(object, 2), 1;...                 % top-right
        size(object, 2), size(object, 1);...   % bottom-right
        1, size(object, 1);...                 % bottom-left
        1, 1];                                 % top-left again to close the polygon
    
new_box_polygon = transformPointsForward(tform, box_polygon);

figure;
imshow(img);
hold on;
line(new_box_polygon(:, 1), new_box_polygon(:, 2), 'Color', 'y');
title('Detected Box');

% Bounding Box
[~, max_i] = max(sum(new_box_polygon,2));
[~, min_i] = min(sum(new_box_polygon,2));
bbox = [new_box_polygon(min_i,:); new_box_polygon(max_i,:)];
bbox = ceil(bbox);
% Put flower on bounding box
flower_img = rgb2gray(imread("flower.jpg"));
flower_img = imresize(flower_img, [bbox(2,2)-bbox(1,2) bbox(2,1)-bbox(1,1)]);
img(bbox(1,2):bbox(2,2)-1,bbox(1,1):bbox(2,1)-1) = flower_img;
figure;
imshow(img);
title('Final Image');


