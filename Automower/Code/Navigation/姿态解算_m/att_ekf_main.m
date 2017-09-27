clc;clear;
close all;

%% load data file
%load by name
file = '0706_indoor_packed_static';
suffix = '.mat';
file_name = [file,suffix];
load(file_name);

%load by ui interface
% [file_name, pathname] = uigetfile('*.mat', '读取数据文件','C:\Users\Administrator\Desktop\Data file'); 
% load(file_name);
% file_name(end-3:end)=[];

% sampling freq
fs = 50;   %[Hz]
dt = 1/fs;

%% Preprocessing: invalid data and outlier remove
preprocessing;

%% Calibration: mag and gyro calibration (accelerometer absent now!!!)
Calibrations;

%% Variables definition and frame alignment
variable_define;
frame_alignment;

%% Initilization
%state vector initialization
stateVec(:,1) = [1,0,0,0]';

%% Parameters 
parameters_setting;

%% loop start
for k = 2:len
%time udpate
[P,stateVec(:,k)] = tu_att_ekf(P,stateVec(:,k-1),angular_imu_rad_s(:,k),dt,Q);

%measurement update
[P,stateVec(:,k)] = mu_att_ekf(P,acc_imu_m_s2(:,k),stateVec(:,k),R);

%calculated euler angle
eul(:,k) = quat2eulf(stateVec(:,k)');

%replay yaw angle by mag result  
pitch = eul(2,k);
roll = eul(3,k);

%tilt compensation(not feasible now due to interface in mz)
% Hxa(k) = body_mx(k)*cos(pitch)+body_my(k)*sin(roll)*sin(pitch)+body_mz(k)*cos(roll)*sin(pitch);
% Hya(k) = body_my(k)*cos(roll)-body_mz(k)*sin(roll);
% Hx2a(k) = body_mx2(k)*cos(pitch)+body_my2(k)*sin(roll)*sin(pitch)+body_mz2(k)*cos(roll)*sin(pitch);
% Hy2a(k) = body_my2(k)*cos(roll)-body_mz2(k)*sin(roll);

%ignore small tile angles
Hx(k) = body_mx(k);
Hy(k) = body_my(k);
Hx2(k) = body_mx2(k);
Hy2(k) = body_my2(k);

%yaw angle by magnetometer: initial phase will be take into account later
yaw_mag(k) = -atan2(Hy(k),Hx(k)); %+ ellipse_t.ellipse_t.phi;
yaw_mag2(k) = -atan2(Hy2(k),Hx2(k));% + ellipse_t2.ellipse_t2.phi;

%take declination into consideration
eul(1,k)  = yaw_mag(k) - delta_heading_rad;

% %calculated new quat
stateVec(:,k) = eul2quatf(eul(:,k)');

%renormalize
stateVec(:,k) = stateVec(:,k)/norm(stateVec(:,k));
norm_P(k) = norm(P);

end

%% plot
%roll and pitch
figure;
subplot(211);plot((1:len)*dt,eul(3,1:len)*180/pi);xlabel('time [s]');ylabel('roll angle [deg]');grid on;title('Euler Angle');
subplot(212);plot((1:len)*dt,eul(2,1:len)*180/pi);xlabel('time [s]');ylabel('pitch angle [deg]');grid on;

%yaw
%yaw range shift
for k=1:len
    if (yaw_mag(k)<0)&&(yaw_mag(k)>-pi-abs(ellipse_t.phi))
        yaw_mag(k) = yaw_mag(k) + pi;
    end
    if (yaw_mag2(k)<0)&&(yaw_mag2(k)>-pi-abs(ellipse_t2.phi))
        yaw_mag2(k) = yaw_mag2(k) + pi;
    end
end

%norm of P matrix
figure;
plot(norm_P);grid on;title('norm P');

%% Save files
quat = stateVec;
yaw_att = eul(1,:);
suffix = '_att.mat';
file = [file_name,suffix];
save(['C:\Users\Administrator\Desktop\Data file\', file], 'quat', 'yaw_att');

%% Magnetic compass analysis
mag_analysis;