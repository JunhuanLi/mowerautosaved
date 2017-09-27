function [pos,vel,eul,P,stateVec_updated] = ekf9_gps_mu(pos_prev,vel_prev,eul_prev,gps_pos_lla,gps_vel_ned,stateVec,P,R,pos_lla_ref)

gps_pos_ecef = wgslla2xyz(gps_pos_lla(1),gps_pos_lla(2),gps_pos_lla(3));
gps_pos_ned = wgsxyz2ned(gps_pos_ecef,pos_lla_ref(1),pos_lla_ref(2),pos_lla_ref(3));
posInnov = gps_pos_ned - pos_prev;

% posInnov = lla2flat([pos_ins(1)*180/pi,pos_ins(2)*180/pi,pos_ins(3)],gps_pos_lla(1:2)*180/pi,0,gps_pos_lla(3))';
% posInnov = lla2flat([pos_ins(1)*180/pi,pos_ins(2)*180/pi,0],gps_pos_lla(1:2)*180/pi,0,0)';

velInnov = gps_vel_ned - vel_prev;
z = [posInnov; velInnov];

H = [eye(6),zeros(6,3)];
K = P * H'/((H * P * H' + R ));
stateVec_updated = stateVec + K*(z - H*stateVec);

% P矩阵更新
P = (eye(9) - K * H)* P;

% 保证P阵为对称阵，防止由于矩阵病态性引起滤波器发散
P = 0.5*(P + transpose(P));

% % % 保证P阵对角线元素为正
for i=1:9
   if P(i,i) < 0
      P(i,i) = 0;
   end
end

%状态量更新    
pos = pos_prev + stateVec_updated(1:3);
vel = vel_prev + stateVec_updated(4:6);
% eul = mod(eul_prev + stateVec_updated(7:9),2*pi);
eul = eul_prev + stateVec_updated(7:9);
end