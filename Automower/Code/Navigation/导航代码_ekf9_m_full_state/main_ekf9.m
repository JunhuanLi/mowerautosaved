%%
%state vector = [delta_pos,del_vel,acc_bias]; 9*1
clear;clc;close all
load data0613_10Hz.mat
load data0613_10Hz_quat.mat

%% 数据预处理
tic
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
     [pos(:,k),vel(:,k),eul(:,k),stateVec(:,k),P,an(:,k)] = ekf9_tu(acc_measurement_m_s2(:,k),pos_prev,vel_prev,quat(k,:),quat(k-1,:),parameters.dt,parameters.P,parameters.Q,parameters.tau_acc);
%      P = P + diag(ones(1,9).^2);
%      stateVec(:,k) = stateVec(:,k-1);

%% GPS位置更新
     R = diag([gps_hAcc_m(k),gps_hAcc_m(k),gps_vAcc_m(k),gps_sAcc_m_s(k),gps_sAcc_m_s(k),gps_sAcc_m_s(k)]).*2;
     [pos(:,k),vel(:,k),eul(:,k),P,stateVec(:,k)] = ekf9_gps_mu(pos(:,k-1),vel(:,k-1),eul(:,k-1),pos_gps_lla(:,k),vel_gps_ned(:,k),stateVec(:,k),P,R,gps_lla_ref);

%% 惯导更新
     %[pos(:,k),vel(:,k),P,an(:,k)] = ekf9_imu_mu(acc_measurement_m_s2(:,k),yaw(k),pitch(k),roll(k),pos_prev,vel_prev,quat(k,:),parameters.dt,parameters.P,parameters.Q,parameters.tau_acc);
%      [pos(:,k),vel(:,k),P,stateVec(:,k)] = ekf9_imu_mu(acc_measurement_m_s2(:,k),yaw(k),pitch(k),roll(k),quat(k,:),parameters.dt,P,parameters.R_imu*10,pos(:,k-1),vel(:,k-1),stateVec(:,k));

%% 里程计更新
     [pos(:,k),vel(:,k),eul(:,k),P,stateVec(:,k)] = ekf9_odom_mu(pos(:,k-1),vel(:,k-1),eul(:,k-1),stateVec(:,k),delta_odo_r(k),delta_odo_l(k),P,parameters.R_odom,parameters.D);

%% 解算pose vector
%    pos(:,k) = pos(:,k-1) + stateVec(1:3,k);
%    vel(:,k) = vel(:,k-1) + stateVec(4:6,k);
%    eul(:,k) = eul(:,k-1) + stateVec(7:9,k);

%% 为下一循环赋值      
%      pos_prev = pos(:,k-1);
%      vel_prev = vel(:,k-1);
     norm_P(k) = norm(P);
     
end
toc
%% Performance evaluation
% figure_output;

% gps position out
%%%%method 1 by own functions, faster
for k = valid_gps_flag:len
pos_gps_ecef(:,k) = wgslla2xyz(pos_gps_lla(1,k),pos_gps_lla(2,k),pos_gps_lla(3,k));
pos_gps_ned(:,k) = wgsxyz2ned(pos_gps_ecef(:,k),gps_lla_ref(1),gps_lla_ref(2),gps_lla_ref(3));
end

% position output
figure;
subplot(211);plot((1:len-1)*parameters.dt,pos(1,:));xlabel('time [s]');ylabel('x position');grid on;hold on;plot((1:len)*parameters.dt,pos_gps_ned(1,:));legend('EKF','GPS');
subplot(212);plot((1:len-1)*parameters.dt,pos(2,:));xlabel('time [s]');ylabel('y position');grid on;hold on;plot((1:len)*parameters.dt,pos_gps_ned(2,:));

figure;
subplot(311);plot((1:len-1)*parameters.dt,eul(3,1:len-1)*180/pi);xlabel('time [s]');ylabel('roll angle [deg]');grid on;title('Euler Angle');
subplot(312);plot((1:len-1)*parameters.dt,eul(2,1:len-1)*180/pi);xlabel('time [s]');ylabel('pitch angle [deg]');grid on;
subplot(313);plot((1:len-1)*parameters.dt,eul(1,1:len-1)*180/pi);xlabel('time [s]');ylabel('yaw angle [deg]');grid on;

figure;
plot(norm_P);grid on;title('norm of P');

% velocity output
figure;
subplot(211);plot((1:len-1)*parameters.dt,vel(1,:));xlabel('time [s]');ylabel('N velocity');grid on;hold on;plot((1:len-1)*parameters.dt,gps_vn_m_s(1:len-1));legend('EKF','GPS');
subplot(212);plot((1:len-1)*parameters.dt,vel(2,:));xlabel('time [s]');ylabel('E velocity');grid on;hold on;plot((1:len-1)*parameters.dt,gps_vn_m_s(1:len-1));


%% output
% for k = 1:length(pos_ins)
% lla(:,k) = flat2lla(pos_ins(:,k)', pos_lla_ref(1:2)*180/pi, 0, pos_lla_ref(3));
% lla(:,k) = flat2lla(pos_ins(:,k)',[31.0992102000000  121.505077000000], 0, pos_lla_ref(3));

