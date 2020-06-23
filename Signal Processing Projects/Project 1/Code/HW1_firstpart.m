% Q1
figure;
t = -2:0.01:2;

% Signals
y1 = sin(2*pi*t); 
y2 = sin(2*pi*10*t); 
y3 = 10*sin(2*pi*t);
y4 = sin(2*pi*t)+10;
y5 = sin(2*pi*(t- 0.5));
y6 = 10*sin(2*pi*10*t);
y7 = t.*sin(2*pi*t);
y8 = sin(2*pi*t)./t;
y9 = y1+y2+y3+y4+y5+y6+y7+y8;

% Plots
subplot(5,2,1);
plot(t,y1);
title("y1");
subplot(5,2,2);
plot(t,y2);
title("y2");
subplot(5,2,3);
plot(t,y3);
title("y3");
subplot(5,2,4);
plot(t,y4);
title("y4");
subplot(5,2,5);
plot(t,y5);
title("y5");
subplot(5,2,6);
plot(t,y6);
title("y6");
subplot(5,2,7);
plot(t,y7);
title("y7");
subplot(5,2,8);
plot(t,y8);
title("y8");
subplot(5,2,9);
plot(t,y9);
title("y9");

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 

% Q2
figure;
z = randn(1,401)*0.1;  % Random signal (Normal)

% Signals
y10 = z;
y11 = z+t;
y12 = z+y1;
y13 = z.*y1;
y14 = t.*sin(2*pi*z);
y15 = sin(2*pi*(t+z));
y16 = z.*y2;
y17 = sin(2*pi*(t+10*z));
y18 = y1./z;
y19 = y11+y12+y13+y14+y15+y16+y17+y18;

% Plots
subplot(5,2,1);
plot(t,y10);
title("y10");
subplot(5,2,2);
plot(t,y11);
title("y11");
subplot(5,2,3);
plot(t,y12);
title("y12");
subplot(5,2,4);
plot(t,y13);
title("y13");
subplot(5,2,5);
plot(t,y14);
title("y14");
subplot(5,2,6);
plot(t,y15);
title("y15");
subplot(5,2,7);
plot(t,y16);
title("y16");
subplot(5,2,8);
plot(t,y17);
title("y17");
subplot(5,2,9);
plot(t,y18);
title("y18");
subplot(5,2,10);
plot(t,y19);
title("y19");

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Q3
figure;
z = rand(1,401)*0.1;  % Random Signal (Uniform)

% Signals
y20 = z;
y21 = z+t;
y22 = z+y1;
y23 = z.*y1;
y24 = t.*sin(2*pi*z);
y25 = sin(2*pi*(t+z));
y26 = z.*y2;
y27 = sin(2*pi*(t+10*z));
y28 = y1./z;
y29 = y21+y22+y23+y24+y25+y26+y27+y28;

% Plots
subplot(5,2,1);
plot(t,y20);
title("y20");
subplot(5,2,2);
plot(t,y21);
title("y21");
subplot(5,2,3);
plot(t,y22);
title("y22");
subplot(5,2,4);
plot(t,y23);
title("y23");
subplot(5,2,5);
plot(t,y24);
title("y24");
subplot(5,2,6);
plot(t,y25);
title("y25");
subplot(5,2,7);
plot(t,y26);
title("y26");
subplot(5,2,8);
plot(t,y27);
title("y27");
subplot(5,2,9);
plot(t,y28);
title("y28");
subplot(5,2,10);
plot(t,y29);
title("y29");

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Q4
figure;

r1 = randn(5000,1);               % mean = 0, var = 1
r2 = sqrt(8).*randn(5000,1);      % mean = 0, var = 8
r3 = sqrt(64).*randn(5000,1);     % mean = 0, var = 64
r4 = sqrt(256).*randn(5000,1);    % mean = 0, var = 256

% Plots
subplot(4,1,1);
histogram(r1);
xlim([-60, 60]);
title("r1");
subplot(4,1,2);
histogram(r2);
xlim([-60, 60]);
title("r2");
subplot(4,1,3);
histogram(r3);
xlim([-60, 60]);
title("r3");
subplot(4,1,4);
histogram(r4);
xlim([-60, 60]);
title("r4");

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Q5
figure;

r6 = randn(5000,1) + 10;            % mean =  10, var = 1
r7 = sqrt(4).*randn(5000,1) + 20;   % mean =  20, var = 4
r8 = randn(5000,1) - 10;            % mean = -10, var = 1
r9 = sqrt(4).*randn(5000,1) - 20;   % mean = -20, var = 4

% Plots
subplot(4,1,1);
histogram(r6);
xlim([-28, 28]);
title("r6");
subplot(4,1,2);
histogram(r7);
xlim([-28, 28]);
title("r7");
subplot(4,1,3);
histogram(r8);
xlim([-28, 28]);
title("r8");
subplot(4,1,4);
histogram(r9);
xlim([-28, 28]);
title("r9");

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Q6
figure;

r11 = rand(5000,1)*8 - 4; 
r21 = rand(5000,1)*40 - 20;

% Plots
subplot(2,1,1);
histogram(r11);
xlim([-20, 20]);
title("r11");
subplot(2,1,2);
histogram(r21);
xlim([-20, 20]);
title("r21");

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
