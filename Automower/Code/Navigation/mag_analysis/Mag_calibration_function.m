clc;clear;
close all;
file = '0705_indoor_rot3';
suffix = '.mat';
file_name = [file,suffix];
load(file_name);

%preprocessing
head = 10;
imu_mx = imu_mx(head:end);
imu_my = imu_my(head:end);
imu_mx2 = imu_mx2(head:end);
imu_my2 = imu_my2(head:end);

%ellipse fitting
ellipse_t = fit_ellipse(imu_mx,imu_my);
ellipse_t2 = fit_ellipse(imu_mx2,imu_my2);

mag_body = mag_calibration_ellipse(ellipse_t,imu_mx,imu_my,imu_mz);
mag_body2 = mag_calibration_ellipse(ellipse_t2,imu_mx2,imu_my2,imu_mz2);

figure;plot(mag_body(1,:),mag_body(2,:));axis equal;grid on;
hold on;plot(mag_body2(1,:),mag_body2(2,:));legend('mag1','mag2');

%save
save(['C:\Users\zh\Desktop\fangrui_code\test data\', 'mapping_ellipse1.mat'],'ellipse_t');
save(['C:\Users\zh\Desktop\fangrui_code\test data\', 'mapping_ellipse2.mat'],'ellipse_t2');
