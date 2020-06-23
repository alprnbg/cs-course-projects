clear,clc,close all;

f1 = 0.20;
f2 = 0.50;
t = 0:0.001:20;      % CT time
x1 = sin(2*pi*f1*t); % First CT signal
x2 = cos(2*pi*f2*t); % Second CT signal

% Undersampling
figure;
plot(t,x1+x2);
hold on;
fs_under = 1*f2;     % Sampling Rate
Ts_under = 1/fs_under;
td_under = 0:Ts_under:20;
x_under = sin(2*pi*f1*td_under) + cos(2*pi*f2*td_under);
stem(td_under, x_under,"Color",[0.91 0.41 0.17], "LineWidth",1.3);
% Reconstruction
y1 = zeros(1,length(t));
samples = length(td_under);
for i = 1:1:length(t)
    for n = 1:1:samples
        y1(i) = y1(i) + x_under(n)*sinc((t(i)-n*Ts_under)/Ts_under);
    end
end
hold on;
plot(t-Ts_under,y1,"Color","y");
xlim([0 20]);
xlabel('Time (sec)');
ylabel('Signal');
title("Reconstruction with Undersampling")
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Sampling at Nyquist Rate
figure;
plot(t,x1+x2);
hold on;
fs_nyquist = 2*f2;    % Sampling Rate
Ts_nyquist = 1/fs_nyquist;
td_nyquist = 0:Ts_nyquist:20;
x_nyquist = sin(2*pi*f1*td_nyquist) + cos(2*pi*f2*td_nyquist);
stem(td_nyquist, x_nyquist,"Color",[0.91 0.41 0.17],"LineWidth",1.3);
% Reconstruction
y2 = zeros(1,length(t));
samples = length(td_nyquist);
for i = 1:1:length(t)
    for n = 1:1:samples
        y2(i) = y2(i) + x_nyquist(n)*sinc((t(i)-n*Ts_nyquist)/Ts_nyquist);
    end
end
hold on;
plot(t-Ts_nyquist,y2,"Color","y");
xlim([0 20]);
xlabel('Time (sec)');
ylabel('Signal');
title("Reconstruction with Sampling at Nyquist Rate")
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Oversampling
figure;
plot(t,x1+x2);
hold on;
fs_over = 5*f2;       % Sampling Rate
Ts_over = 1/fs_over;
td_over = 0:Ts_over:20;
x_over = sin(2*pi*f1*td_over) + cos(2*pi*f2*td_over);
stem(td_over, x_over,"Color",[0.91 0.41 0.17], "LineWidth",1.3);
% Reconstruction
y3 = zeros(1,length(t));
samples = length(td_over);
for i = 1:1:length(t)
    for n = 1:1:samples
        y3(i) = y3(i) + x_over(n)*sinc((t(i)-n*Ts_over)/Ts_over);
    end
end
hold on;
plot(t-Ts_over,y3,"Color","y");
xlim([0 20]);
xlabel('Time (sec)');
ylabel('Signal');
title("Reconstruction with Oversampling")