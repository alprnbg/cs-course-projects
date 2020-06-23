clear,clc,close all;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure;
subplot(2,1,1);
fs = 1000;
t = 0:1/fs:0.12;
x = (sawtooth(2*pi*25*t,0.5)+1)./2;
plot(t,x);
xlabel('Time (sec)');
ylabel('Signal');
title("Triangle Wave with T0=0.04s")
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
subplot(2,1,2);
y = fftshift(fft(x));
n = length(x);
f0 = (-n/2:n/2-1)*(fs/n);
normalized = abs(y/n);
plot(f0,normalized);
xlabel('Frequency (1/s)');
ylabel('Value');
title("Output of fftshift")
xlim([-350 350])
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure;
fs = 1000;
t = 0:1/fs:0.12;

subplot(2,1,1)
x = t.*0;
x = x + 0.5; % DC component
for k = 1:3
    if mod(k,2)==1
        x = x + (-4/((pi*k)^2))*cos(2*pi*25*k*t);  
    end
end
plot(t,x);
xlabel('Time (sec)');
ylabel('Signal');
title("Sum of DC and 1st through 3rd Harmonics")
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
subplot(2,1,2)
x = t.*0;
x = x + 0.5; % DC component
for k = 1:11
    if mod(k,2)==1
        x = x + (-4/((pi*k)^2))*cos(2*pi*25*k*t);  
    end
end
plot(t,x);
xlabel('Time (sec)');
ylabel('Signal');
title("Sum of DC and 1st through 11th Harmonics")