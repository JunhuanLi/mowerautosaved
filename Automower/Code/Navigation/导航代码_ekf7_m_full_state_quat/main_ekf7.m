%state vector = [delta_pos,del_vel,delta_yaw]; 9*1
clear;clc;close all
file = '0705_4_grass_walk';
suffix = '.mat';
file_name = [file,suffix];
load(file_name);
% [file, pathname] = uigetfile('*.mat', '读取数据文件','C:\Users\Administrator\Desktop\Data file'); 
% load(file);
% file(end-3:end)=[];

suffix = '_quat.mat';
file_name = [file,suffix];
load(file_name);
suffix = '_yaw.mat';
file_name = [file,suffix];
load(file_name);
%% 数据预处理
data_preprocessing;

%% 参数设置
parameters_setting;

%% 变量定义
variables_define;

%% 低通滤波器
% low_pass_filtering;

%% 
for k = 2:len-1
%% 防止四元数越界
     if abs(norm(quat(k-1,:))-1) > 1e-6
        quat(k-1,:) = quat_prev;
     else
         quat_prev = quat(k-1,:);
     end
     
%% 时间更新
     [pos(:,k),vel(:,k),yaw(k),stateVec(:,k),P] = ekf7_tu(acc_measurement_m_s2(:,k),pos(:,k-1),vel(:,k-1),yaw(:,k-1),quat(k,:),parameters.dt,P,parameters.Q,yaw_att(k));
%      P1 = P;
%      yaw(k)
%% GPS位置更新
if (gps_state(k)>=2)
     R_gps7 = diag([gps_hAcc_m(k),gps_hAcc_m(k),gps_vAcc_m(k),gps_sAcc_m_s(k),gps_sAcc_m_s(k),gps_sAcc_m_s(k),gps_headAcc_deg(k)/180*pi]).*2;
     R_gps6 = diag([gps_hAcc_m(k),gps_hAcc_m(k),gps_vAcc_m(k),gps_sAcc_m_s(k),gps_sAcc_m_s(k),gps_sAcc_m_s(k)]);
     %gps更新7个状态量
%      [pos(:,k),vel(:,k),yaw(:,k),P,stateVec(:,k)] = ekf7_gps7_mu(pos(:,k-1),vel(:,k-1),yaw(:,k-1),pos_gps_lla(:,k),vel_gps_ned(:,k),gps_heading_deg(k),stateVec(:,k),P,R_gps7,gps_lla_ref);
     %gps不更新heading
     [pos(:,k),vel(:,k),yaw(k),P,stateVec(:,k)] = ekf7_gps6_mu(pos(:,k-1),vel(:,k-1),yaw(k-1),pos_gps_lla(:,k),vel_gps_ned(:,k),stateVec(:,k),P,R_gps6,gps_lla_ref);
end

%% 惯导更新
%      [pos(:,k),vel(:,k),P,stateVec(:,k)] = ekf9_imu_mu(acc_measurement_m_s2(:,k),yaw(k),pitch(k),roll(k),quat(k,:),parameters.dt,P,parameters.R_imu*10,pos(:,k-1),vel(:,k-1),stateVec(:,k));

%% 里程计更新
     % 里程计更新pos_x,pos_y,vx,vy,theta
%      [pos(:,k),vel(:,k),yaw(:,k),P,stateVec(:,k),delta_theta(k)] = ekf7_odom5_mu(pos(:,k-1),vel(:,k-1),yaw(:,k-1),stateVec(:,k),delta_odo_r(k),delta_odo_l(k),P,parameters.R_odom5,parameters.D,parameters.dt);
%      里程计更新pos_x,pos_y,theta
     [pos(:,k),vel(:,k),yaw(k),P,stateVec(:,k),delta_theta(k)] = ekf7_odom3_mu(pos(:,k-1),vel(:,k-1),yaw(k-1),stateVec(:,k),delta_odo_r(k),delta_odo_l(k),P,parameters.R_odom3,parameters.D,parameters.dt);
     
%% 罗盘更新

%% 计算norm P,判读滤波器健康状况
     norm_P(k) = norm(P);
     
end

%% Performance evaluation
% figure_output;

% gps position out
for k = n_valid:len
pos_gps_ecef(:,k) = wgslla2xyz(pos_gps_lla(1,k),pos_gps_lla(2,k),pos_gps_lla(3,k));
pos_gps_ned(:,k) = wgsxyz2ned(pos_gps_ecef(:,k),gps_lla_ref(1),gps_lla_ref(2),gps_lla_ref(3));
end

% position output
% figure;
% subplot(211);plot((1:len-1)*parameters.dt,pos(1,:));xlabel('time [s]');ylabel('x position');grid on;hold on;plot((1:len)*parameters.dt,pos_gps_ned(1,:));legend('EKF','GPS');
% subplot(212);plot((1:len-1)*parameters.dt,pos(2,:));xlabel('time [s]');ylabel('y position');grid on;hold on;plot((1:len)*parameters.dt,pos_gps_ned(2,:));

%yaw output
figure;plot((1:len-1)*parameters.dt,yaw*180/pi);hold on;plot((1:len-1)*parameters.dt,yaw_att(1:len-1)*57.3);
hold on;plot((1:len-1)*parameters.dt,gps_heading_deg(1:len-1));
legend('ekf','mag','GPS heading');grid on;
xlabel('Time [s]');ylabel('Yaw angle [deg]')

% velocity output
figure;
subplot(211);plot((1:len-1)*parameters.dt,vel(1,:));xlabel('time [s]');ylabel('N velocity');grid on;hold on;plot((1:len-1)*parameters.dt,gps_vn_m_s(1:len-1));legend('EKF','GPS');
subplot(212);plot((1:len-1)*parameters.dt,vel(2,:));xlabel('time [s]');ylabel('E velocity');grid on;hold on;plot((1:len-1)*parameters.dt,gps_ve_m_s(1:len-1));

figure;
plot(norm_P);grid on;title('norm of P');

% figure;
plot(pos(2,:),pos(1,:));xlabel('E position [m]');ylabel('N position [m]');grid on;hold on;
plot(pos_gps_ned(2,:),pos_gps_ned(1,:));legend('EKF','GPS');
axis equal

%% output
% for k = 1:length(pos_ins)
% lla(:,k) = flat2lla(pos_ins(:,k)', pos_lla_ref(1:2)*180/pi, 0, pos_lla_ref(3));
% lla(:,k) = flat2lla(pos_ins(:,k)',[31.0992102000000  121.505077000000], 0, pos_lla_ref(3));

