clear;clc;close all;
%
load mag_border_motor_test_CH.mat
len = 228;
figure;
plot(imu_mx_chunhua_off_motor_off(1:len));hold on;plot(imu_mx_chunhua_off_motor_on(1:len));
hold on;plot(imu_mx_chunhua_on_motor_off(1:len));hold on;plot(imu_mx_chunhua_on_motor_on(1:len));
grid on;legend('border off motor off','border off motor on','border on motor off','all on');

% imu_mx_chunhua_off_motor_off = wden(imu_mx_chunhua_off_motor_off,'heursure','s','sln',6,'db2');
% imu_mx_chunhua_off_motor_on = wden(imu_mx_chunhua_off_motor_on,'heursure','s','sln',6,'db2');
% imu_my_chunhua_on_motor_off = wden(imu_my_chunhua_on_motor_off,'heursure','s','sln',6,'db2');
% imu_mx_chunhua_on_motor_on = wden(imu_mx_chunhua_on_motor_on,'heursure','s','sln',6,'db2');


% yaw = atan2(imu_my_chunhua_on_motor_off,imu_mx_chunhua_on_motor_off);
% figure;plot(yaw*57.3);grid on;title('Yaw angle [deg]')

figure;
plot(imu_mx_chunhua_off_motor_off(1:len));hold on;plot(imu_mx_chunhua_off_motor_on(1:len));
hold on;plot(imu_mx_chunhua_on_motor_off(1:len));hold on;plot(imu_mx_chunhua_on_motor_on(1:len));
grid on;legend('border off motor off','border off motor on','border on motor off','all on');

% 
% %%butterworth
% Wn = 1/10;  % normalized cutoff frequency
% N = 2;                    % N-order filter
% [b,aa] = butter(N,Wn,'low');
% imu_mx_lp_chunhua_off_motor_on = filter(b,aa,imu_mx_chunhua_off_motor_on);
% figure;
% plot(imu_mx_chunhua_off_motor_on(7:len));hold on;plot(imu_mx_lp_chunhua_off_motor_on(7:len));grid on;title('mx');legend('mx','low-pass filterd mx');
% 
% [pxx,f1]=pwelch(imu_mx_chunhua_on_motor_off,[],[],[],10,'onesided');
% L = length(imu_mx_chunhua_off_motor_on);
% NFFT = 2^nextpow2(L);
% Y = fft(imu_mx_chunhua_off_motor_on,NFFT)/L;
% f = 10/2*linspace(0,1,NFFT/2+1);
% figure;
% loglog(f1,pxx);grid on;