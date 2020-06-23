function [output] = Conv2D(input, kernel)
    %Conv2D Applies the convolution operation on given image using given
    %       kernel. This function uses "same padding", that is, the shape 
    %       of output and input is the same. In addition, zero padding is 
    %       applied. Kernel must be given in 2D.
    
    output = zeros(size(input));
    k_size = size(kernel);
    in_size = size(input);
    
    % Zero Padding
    pad_amount = (k_size(1)-1)/2;
    padded_in = zeros(in_size(1)+2*pad_amount, in_size(2)+2*pad_amount,3);
    padded_in(pad_amount+1:pad_amount+in_size(1),pad_amount+1:pad_amount+in_size(2),:) = input(:,:,:);
    
    % Make kernel 3D
    kernel_3d = zeros(k_size(1),k_size(1),3);
    for i = 1:3
        kernel_3d(:,:,i) = kernel;
    end
    
    % Sliding the kernel over the image and clip the values outside the
    % range of [0,1]
    for i = 1:in_size(1)
        for j = 1:in_size(2)
            output(i,j,:) = sum(sum(kernel_3d.*padded_in(i:i+k_size(1)-1,j:j+k_size(2)-1,:)));
            output(i,j,:) = max(min(1,output(i,j,:)),0);
        end
    end
end