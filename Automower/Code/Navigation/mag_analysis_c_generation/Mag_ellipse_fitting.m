clc;clear;close all;
load mag_data.mat;

%preprocessing
len = 500;
imu_mx = imu_mx(1:len);
imu_my = imu_my(1:len);

%ellipse fitting
ellipse_t = fit_ellipse(imu_mx,imu_my);

%mapping to circle
% phi = ellipse_t.phi;
% B = [ellipse_t.X0;ellipse_t.Y0];
% Xf = max(1,ellipse_t.b/ellipse_t.a);
% Yf = max(1,ellipse_t.a/ellipse_t.b);
% % T = diag([1/Xf,1/Yf]);   
% T = diag([1,ellipse_t.a/ellipse_t.b]);
% A = [cos(phi) -sin(phi);sin(phi) cos(phi)];

% %ellipse conversion
% mag_body = zeros(3,length(imu_mx));
% for k = 1:length(imu_mx)
%     mag_body(1:2,k) = T * A * [imu_mx(k);imu_my(k)] - B;
%     mag_body(3,k) = imu_mz(k);
% end

