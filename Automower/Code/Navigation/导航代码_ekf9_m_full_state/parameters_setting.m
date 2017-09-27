%parameters
% sigma_an = [300 300 300]*1e-6*GRAV;      %m/s2
% sigma_aw = [0.01,0.01,0.01]*.001;             %m/s2/sqrt(s)
sigma_an = [0.2,0.2,0.2];      %m/s2
sigma_aw = [0.05,0.05,0.05];   %m/s2/sqrt(s)
fs = 10;   %[Hz]
GRAV = 9.8005;

parameters.dt = 1/fs;
parameters.tau_acc = 6;
parameters.P = eye(9)*10;
parameters.Q = diag([sigma_an].^2)*1;
% parameters.Q = diag([1.38579984,1.38579984,1.38579984,1.60393500000000e-07,1.60393500000000e-07,1.60393500000000e-07])*1;
parameters.R_imu = diag(ones(1,9)*4);
parameters.R_odom = diag([0.01,0.01,0.1])*3;
P = parameters.P;
parameters.D = 0.75;   %vehicle diameter