function [pos,vel,yaw,P,stateVec_updated,delta_theta] = ekf7_odom3_mu(pos_prev,vel_prev,yaw_prev,stateVec,delta_odo_r,delta_odo_l,P,R,D,dt)
%yaw clockwise
theta = yaw_prev;   %eul = [yaw,pitch,roll] in the order of ZYX
r = delta_odo_r;
l = delta_odo_l;
if r~=l
    alpha = (l-r)/D;
    delta_x = (D+2*l/alpha)*cos(theta+alpha/2)*sin(alpha/2);
    delta_y = (D+2*l/alpha)*sin(theta+alpha/2)*sin(alpha/2);
    delta_theta = alpha;
else
    alpha = 0;
    delta_x = cos(theta)*r;
    delta_y = sin(theta)*r;
    delta_theta = alpha;      
end
z = [delta_x;delta_y;-delta_theta];

delta_x = cos(theta+(r-l)/2/D)*0.5*(r+l);
delta_y = sin(theta+(r-l)/2/D)*0.5*(r+l);
delta_theta = (l-r)/D;
z = [delta_x;delta_y;-delta_theta];
    
H = [eye(3),zeros(3,4)];
H(3,3) = 0;
H(3,7) = 1;
K = P * H'/((H * P * H' + R ));
stateVec_updated = stateVec + K*(z - H*stateVec);

% P矩阵更新
P = (eye(length(P)) - K * H)* P;

% 保证P阵为对称阵，防止由于矩阵病态性引起滤波器发散
P = 0.5*(P + transpose(P));

% % % 保证P阵对角线元素为正

for i=1:length(P)
   if P(i,i) < 0
      P(i,i) = 0;
   end
end

%状态量更新    
pos = pos_prev + stateVec_updated(1:3);
vel = vel_prev + stateVec_updated(4:6);
yaw = yaw_prev + stateVec_updated(7);
yaw = mod(yaw,2*pi);
%  pos_prev'
%  stateVec_updated(1:3)'
%   pos'
end