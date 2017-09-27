function [pos,vel,acc_bias,P,stateVec] = ekf9_gps_mu(pos,vel,gps_pos_lla,gps_vel_ned,acc_bias,P,R,pos_lla_ref)

gps_pos_ecef = wgslla2xyz(gps_pos_lla(1),gps_pos_lla(2),gps_pos_lla(3));
gps_pos_ned = wgsxyz2ned(gps_pos_ecef,pos_lla_ref(1),pos_lla_ref(2),pos_lla_ref(3));
posInnov = pos - gps_pos_ned;

% posInnov = lla2flat([pos_ins(1)*180/pi,pos_ins(2)*180/pi,pos_ins(3)],gps_pos_lla(1:2)*180/pi,0,gps_pos_lla(3))';
% posInnov = lla2flat([pos_ins(1)*180/pi,pos_ins(2)*180/pi,0],gps_pos_lla(1:2)*180/pi,0,0)';

velInnov = vel - gps_vel_ned;
stateInnov = [posInnov; velInnov];

H = [eye(6),zeros(6,3)];
% H = h*stateVec_delta_stateVec;    %derivative of h w.r.t. delta_x

K = P * H'/((H * P * H' + R ));
stateVec = K*stateInnov;

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
pos(1) = pos(1) - stateVec(1);
pos(2) = pos(2) - stateVec(2);
pos(3) = pos(3) - stateVec(3);

vel = vel - stateVec(4:6);
acc_bias = acc_bias - stateVec(7:9);

end