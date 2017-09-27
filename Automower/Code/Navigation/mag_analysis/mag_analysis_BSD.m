clear;clc;close all

load mag_border_motor_test_BSD.mat
len = 228;

figure;
plot(imu_mx_border_off_motor_off(1:len));hold on;plot(imu_mx_border_off_motor_on(1:len));
hold on;plot(imu_mx_border_on_motor_off(1:len));hold on;plot(imu_mx_border_on_motor_on(1:len));
grid on;legend('border off motor off','border off motor on','border on motor off','all on');

% imu_mx_border_off_motor_off = medfilt1(imu_mx_border_off_motor_off,11);
% imu_mx_border_off_motor_on = medfilt1(imu_mx_border_off_motor_on,11);
% imu_mx_border_on_motor_off = medfilt1(imu_mx_border_on_motor_off,11);
% imu_mx_border_on_motor_on = medfilt1(imu_mx_border_on_motor_on,11);

% imu_mx_border_off_motor_off = wden(imu_mx_border_off_motor_off,'heursure','s','sln',6,'db2');
% imu_mx_border_off_motor_on = wden(imu_mx_border_off_motor_on,'heursure','s','sln',6,'db2');
% imu_mx_border_on_motor_off = wden(imu_mx_border_on_motor_off,'heursure','s','sln',6,'db2');
% imu_mx_border_on_motor_on = wden(imu_mx_border_on_motor_on,'heursure','s','sln',6,'db2');
% 
% figure;
% plot(imu_mx_border_off_motor_off(1:len));hold on;plot(imu_mx_border_off_motor_on(1:len));
% hold on;plot(imu_mx_border_on_motor_off(1:len));hold on;plot(imu_mx_border_on_motor_on(1:len));
% grid on;legend('border off motor off','border off motor on','border on motor off','all on');


%%butterworthe
Wn = 1/10;  % normalized cutoff frequency
N = 2;                    % N-order filter
[b,aa] = butter(N,Wn,'low');
imu_mx_lp_border_off_motor_on = filter(b,aa,imu_mx_border_off_motor_on);
figure;
plot(imu_mx_border_off_motor_on(7:len));hold on;plot(imu_mx_lp_border_off_motor_on(7:len));
grid on;title('mx');legend('mx','low-pass filterd mx');
% 
[pxx,f1]=pwelch(imu_mx_border_off_motor_on,[],[],[],10,'onesided');
L = length(imu_mx_border_off_motor_on);
NFFT = 2^nextpow2(L);
Y = fft(imu_mx_border_off_motor_on,NFFT)/L;
f = 10/2*linspace(0,1,NFFT/2+1);
figure;
loglog(f1,pxx);grid on;