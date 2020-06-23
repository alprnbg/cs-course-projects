clear,clc,close all;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Read Auodio
[x_noisy,fs_noisy] = audioread('noisy.wav');
[x_clean,fs_clean] = audioread('clean.wav');
l_noisy=length(x_noisy);
l_clean=length(x_clean);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Construct Filter and Get Output

% 5-pt Averaging Filter
%pt = 5;
%filter = ones([1 pt]).*(1/pt);

% Trained filter
filter = [0.15, 0.24, 0.00, 0.35, 0.01];

output = cconv(filter, x_noisy, length(x_noisy));
audiowrite("output.wav",output,fs_noisy);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Noisy Signal FFT
figure;
subplot(3,1,1);
X_noisy = fftshift(fft(x_noisy));
f_noisy = (-l_noisy/2:l_noisy/2-1)*(fs_noisy/l_noisy);
noisy_fourier = abs(X_noisy/l_noisy);    
plot(f_noisy,noisy_fourier);
xlim([-2e3 2e3]);
xlabel('Frequency (1/s)');
ylabel('Value');
title("Noisy FFT")
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Clean Signal FFT
subplot(3,1,2);
X_clean = fftshift(fft(x_clean));
f_clean = (-l_clean/2:l_clean/2-1)*(fs_clean/l_clean);
clean_fourier = abs(X_clean/l_clean);    
plot(f_clean,clean_fourier);
xlim([-2e3 2e3]);
xlabel('Frequency (1/s)');
ylabel('Value');
title("Clean FFT")
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Output Signal FFT
subplot(3,1,3);
%plot(f_noisy, clean_fourier./noisy_fourier);
X_output = fftshift(fft(output));
f_output = (-l_noisy/2:l_noisy/2-1)*(fs_noisy/l_noisy);
output_fourier = abs(X_output/l_noisy);    
plot(f_output,output_fourier);
xlim([-2e3 2e3]);
xlabel('Frequency (1/s)');
ylabel('Value');
title("Filter Output FFT")
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% In order to see the affect of filter in a detailed way, I clipped the signal.
% Clipped Noisy Signal
figure;
subplot(3,1,1);
plot(x_noisy);
xlim([3e4 4e4]);
xlabel('Sample');
ylabel('Value');
title("Clipped Noisy Signal")
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Clipped Clean Signal
subplot(3,1,2);
plot(x_clean);
xlim([3e4 4e4]);
xlabel('Sample');
ylabel('Value');
title("Clipped Clean Signal")
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Clipped Output Signal
subplot(3,1,3);
plot(output);
xlim([3e4 4e4]);
xlabel('Sample');
ylabel('Value');
title("Clipped Filter Output Signal")
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
