function [pos,vel,yaw,P,stateVec_updated] = ekf7_gps6_mu(pos_prev,vel_prev,yaw_prev,gps_pos_lla,gps_vel_ned,stateVec,P,R,pos_lla_ref)

gps_pos_ecef = wgslla2xyz(gps_pos_lla(1),gps_pos_lla(2),gps_pos_lla(3));
gps_pos_ned = wgsxyz2ned(gps_pos_ecef,pos_lla_ref(1),pos_lla_ref(2),pos_lla_ref(3));
posInnov = gps_pos_ned - pos_prev;
velInnov = gps_vel_ned - vel_prev;
z = [posInnov; velInnov];

H = [eye(6),zeros(6,1)];
K = P * H'/((H * P * H' + R ));
stateVec_updated = stateVec + K*(z - H*stateVec);

% P�������
P = (eye(length(P)) - K * H)* P;

% ��֤P��Ϊ�Գ��󣬷�ֹ���ھ���̬�������˲�����ɢ
P = 0.5*(P + transpose(P));

% % % ��֤P��Խ���Ԫ��Ϊ��
for i=1:length(P)
   if P(i,i) < 0
      P(i,i) = 0;
   end
end

%״̬������    
pos = pos_prev + stateVec_updated(1:3);
vel = vel_prev + stateVec_updated(4:6);
yaw = yaw_prev + stateVec_updated(7);
end