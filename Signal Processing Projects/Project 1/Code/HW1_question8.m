% I took the scripts shown in ps as reference in this question

[y,fs] = audioread('music.wav'); %y=sampled data and Fs is the sampling rate
l=length(y);

Y = fft(y);               % discrete fourier transform
f = (0:l-1)*(fs/l);       % frequency range
power = abs(Y).^2/l;

subplot(2,1,1);

plot(f,power);
xlabel('Frequency (fft(x))');
ylabel('Power');

Y0 = fftshift(Y);         % shift y values
f0 = (-l/2:l/2-1)*(fs/l); % 0-centered frequency range
power0 = abs(Y0).^2/l;    % 0-centered power

subplot(2,1,2);

plot(f0,power0);
xlabel('Frequency (fftshift(fft(x))');
ylabel('Power');
xlim([-500 500]);


