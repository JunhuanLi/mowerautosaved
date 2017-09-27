clear;clc;close all

%% data input
load data0616_10Hz.mat
load data0616_10Hz_quat.mat
GRAV = 9.8005;

%% Programme Description:
%p:position,v:velocity,q:quaternion,,g:gravity vector
%ab:acclerometer bias,aw:accelerometer bias noise
%wb:imu_angular_yroscope bias,ww:imu_angular_yroscope bias noise
%am:acceleration measurement,an:acceleration noise
%wm:angular rate measurement,wn:angular rate noise
%w = wm - wb
%delta_w = -delta_wb - wn
%note:quaternion in Hamilton form, i.e., q = [qw,qv];
%HDOP ¨C horizontal dilution of precision
%VDOP ¨C vertical dilution of precision
%PDOP ¨C position (3D) dilution of precision

%reference frame: NED frame
%full state:  x = [p,v,ab,g]
%delta state£ºdelta_x = [delta_p,delta_v,delta_ab,delta_g];

%measurements units:
%gps_pos_lla:[rad,rad,m]
%gps_vel_ned:[m/s,m/s,m/s]
%wm: [rad/s]
%am:[m/s^2]

%% data preprocessing and calibration
%remove head invalid values && align imu coordinate frame and board
%coordiate frame
head = 10;
imu_ax_g = -imu_ax_g(head:end);
imu_ay_g = imu_ay_g(head:end);
imu_az_g = -imu_az_g(head:end);
% imu_gx_rps = -imu_gx_rps(head:end);
% imu_gy_rps = imu_gy_rps(head:end);
% imu_gz_rps = -imu_gz_rps(head:end);
imu_mx = -imu_mx;
% quat = quat(:,head:end);
gps_state = gps_state(head:end);
gps_lat_rad = gps_lat_rad(head:end);
gps_lon_rad = gps_lon_rad(head:end);
gps_height_m = gps_height_m(head:end);
gps_vn_m_s = gps_vn_m_s(head:end);
gps_ve_m_s = gps_ve_m_s(head:end);
gps_vd_m_s = gps_vd_m_s(head:end);
gps_vAcc_m = gps_vAcc_m(head:end);
gps_hAcc_m = gps_hAcc_m(head:end);
gps_sAcc_m_s = gps_sAcc_m_s(head:end);

valid_gps_flag = find(gps_state(:)>=2,1,'first');  %???????????
% gps_lla_ref = [gps_lat_rad(valid_gps_flag),gps_lon_rad(valid_gps_flag),gps_altitude(valid_gps_flag)]';

%mediate value filter to remove outliers
%notice:gps altitude not accurate and with outliers appearance
% gps_altitude = medfilt1(gps_altitude,15);
% att_roll_rad = medfilt1(att_roll_rad,7);
% att_pitch_rad = medfilt1(att_pitch_rad,7);
% att_yaw_rad = medfilt1(att_yaw_rad,7);

%hampel function to remove outliers
gps_hAcc_m = hampel(gps_hAcc_m);
gps_vAcc_m = hampel(gps_vAcc_m);
gps_sAcc_m_s = hampel(gps_sAcc_m_s);
gps_lat_rad = hampel(gps_lat_rad);
gps_lon_rad = hampel(gps_lon_rad);
gps_height_m = hampel(gps_height_m);
gps_vn_m_s = hampel(gps_vn_m_s);
gps_ve_m_s = hampel(gps_ve_m_s);
gps_vd_m_s = hampel(gps_vd_m_s);
imu_ax_g = hampel(imu_ax_g);
imu_ay_g = hampel(imu_ay_g);
imu_az_g = hampel(imu_az_g);

% calibration;
for k = 1:length(imu_ax_g)
acc_measurement_norm(:,k) = [imu_ax_g(k) imu_ay_g(k) imu_az_g(k)]'/norm([imu_ax_g(k) imu_ay_g(k) imu_az_g(k)]);          %[m/s2]
end
acc_measurement_m_s2 = acc_measurement_norm*GRAV;
clear imu_ax_g imu_ay_g imu_az_g imu_gx_rps imu_gy_rps imu_gz_rps

%preprocessing
pos_gps_lla = [gps_lat_rad'; gps_lon_rad'; gps_height_m'];
gps_lla_ref = [gps_lat_rad,gps_lon_rad,gps_height_m]';
vel_gps_ned = zeros(3,length(pos_gps_lla));
%low pass filter/mid-value filter
%%%%%TBD
%outliers remove
%%%%TBD

%% initialization
stateVec = zeros(12,length(acc_measurement_m_s2));                                                                            
stateVec(10:12,1) = -acc_measurement_m_s2(:,1); 
delta_stateVec = zeros(12,1);

%parameters
% sigma_an = [300 300 300]*1e-6*GRAV;      %m/s2
% sigma_aw = [0.01,0.01,0.01]*.001;             %m/s2/sqrt(s)
sigma_an = [0.2,0.2,0.2];      %m/s2
sigma_aw = [0.05,0.05,0.05];   %m/s2/sqrt(s)
fs = 10;   %[Hz]
dt = 1/fs;
% Q = diag([sigma_an,sigma_aw].^2)*dt^2;
Q = diag([sigma_an,sigma_aw].^2)*20;
P = eye(12)*10;

%% loop settings
% len = 10000;
len = length(acc_measurement_m_s2) - head;

%% loop start
for k = 2:len
%% time udpate
[P,delta_stateVec,stateVec(:,k)] = tu_12(P,delta_stateVec,stateVec(:,k-1),acc_measurement_m_s2(:,k),dt,Q,quat(:,k));

%% gps position and velocity correction
 if gps_state(k)>=2
    R_gps = diag(0.1*[gps_hAcc_m(k-1),gps_hAcc_m(k-1),gps_vAcc_m(k-1),gps_sAcc_m_s(k-1),gps_sAcc_m_s(k-1),gps_sAcc_m_s(k-1)]).*2;
    [K_norm(k),P,delta_stateVec,stateVec(:,k)] = gps_mu_12(P,delta_stateVec,pos_gps_lla(:,k),vel_gps_ned(:,k),stateVec(:,k),R_gps,gps_lla_ref);
 end

%% calculated euler angle
% eul(:,k) = quat2eul(stateVec(7:10,k)');
norm_P(k) = norm(P);

end

%% plot
figure_output;

%% comparison of yaw angle
% yaw_calcu_deg = atan2(imu_my,-imu_mx)*57.3;
% %low pass filter 
% Wn = 2/fs;
% N = 4;
% [b aa] = butter(N,Wn,'low');
% yaw_calcu_deg = filter(b,aa,yaw_calcu_deg);
% %outlier remove
% yaw_calcu_deg = medfilt1(yaw_calcu_deg,11);
% att_yaw_rad = medfilt1(att_yaw_rad,5);
% 
% figure;plot(yaw_calcu_deg);hold on;plot(att_yaw_rad*57.3);grid on;legend('Calculated','EVK');
%EVK output yaw with several outliers

