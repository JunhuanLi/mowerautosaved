function [pos,vel,P_updated,stateVec_updated]=ekf9_imu_mu(acc,yaw,pitch,roll,quat,dt,P,R,pos_prev,vel_prev,stateVec)
%% ������
axis_a = 6378137;             %���������뾶[m]

%% �������������������
g_n  = [0,0,9.8005]';

%% λ�����ٶȽ���
Cnb = quat2dcm(quat)';
[yaw2,pitch2,roll2] = quat2angle(fliplr(quat));
% acc_biased = acc + acc_bias;
a_n = Cnb*acc + g_n  ;
vel = vel_prev + a_n*dt;   
z(4:6) = a_n*dt;   
z(1:3) = dt*0.5*(vel(1:3) + vel_prev(1:3));
z(7:9) = [yaw2,pitch2,roll2]' - [yaw,pitch,roll]';

%% Kalman filter time update
H = eye(9);
K = P * H'/((H * P * H' + R ));
stateVec_updated = stateVec + K*(z' - H*stateVec);
stateVec'
stateVec_updated'
% P�������
P = (eye(9) - K * H)* P;

% ��֤P��Ϊ�Գ��󣬷�ֹ���ھ���̬�������˲�����ɢ
P = 0.5*(P + transpose(P));

% % % ��֤P��Խ���Ԫ��Ϊ��
for i=1:9
   if P(i,i) < 0
      P(i,i) = 0;
   end
end

P_updated = P;
pos = pos_prev + stateVec(1:3);
vel = vel_prev + stateVec(4:6);
end