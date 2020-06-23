% I took the scripts shown in ps as reference in this question

figure;
load("mysignal.mat");

subplot(3,1,1);
plot(t,x);
xlabel('time');
ylabel('signal');

y = fft(x);             %discrete fourier transform

n = length(x);          % number of samples
f = (0:n-1)*(fs/n);     % frequency range
power = abs(y).^2/n;    % power of the DFT
subplot(3,1,2);
plot(f,power);
xlabel('Frequency (fft(x))');
ylabel('Power');

y0 = fftshift(y);         % shift y values
f0 = (-n/2:n/2-1)*(fs/n); % 0-centered frequency range
power0 = abs(y0).^2/n;    % 0-centered power

subplot(3,1,3);

plot(f0,power0);
xlabel('Frequency (fftshift(fft(x))');
ylabel('Power');

