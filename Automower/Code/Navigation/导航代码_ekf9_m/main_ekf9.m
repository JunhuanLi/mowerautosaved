%%
%state vector = [delta_pos,del_vel,acc_bias]; 9*1
clear;clc;close all
load data0616_10Hz.mat
load data0616_10Hz_quat.mat

%% ����Ԥ����
tic
data_preprocessing;
toc
%% ��������
parameters_setting;

%% ��������
variables_define;

%% ��ͨ�˲���
% low_pass_filtering;

%% 
for k = 2:len
%% ��ֹ��Ԫ��Խ��
     if abs(norm(quat(k-1,:))-1) > 1e-6
        quat(k-1,:) = quat_prev;
     else
         quat_prev = quat(k-1,:);
     end
     
%% �ߵ������ʱ�����
     [pos(:,k-1),vel(:,k-1),P,an(:,k-1)] = ekf9_tu(acc_measurement_m_s2(:,k-1),acc_bias,pos_prev,vel_prev,quat(k-1,:),parameters.dt,parameters.P,parameters.Q,parameters.tau_acc);

%% GPSλ�ø���
      R = diag([gps_hAcc_m(k-1),gps_hAcc_m(k-1),gps_vAcc_m(k-1),gps_sAcc_m_s(k-1),gps_sAcc_m_s(k-1),gps_sAcc_m_s(k-1)]).*2*25;
      [pos(:,k-1),vel(:,k-1),acc_bias,P,stateVec(:,k-1)] = ekf9_gps_mu(pos(:,k-1),vel(:,k-1),pos_gps_lla(:,k-1),vel_gps_ned(:,k-1),acc_bias,P,R,gps_lla_ref);
      
%% Ϊ��һѭ����ֵ      
     pos_prev = pos(:,k-1);
     vel_prev = vel(:,k-1);
     norm_P(k) = norm(P);
     
end

%% Performance evaluation
figure_output;
figure;plot(an(3,:));

%% output
% for k = 1:length(pos_ins)
% lla(:,k) = flat2lla(pos_ins(:,k)', pos_lla_ref(1:2)*180/pi, 0, pos_lla_ref(3));
% lla(:,k) = flat2lla(pos_ins(:,k)',[31.0992102000000  121.505077000000], 0, pos_lla_ref(3));

