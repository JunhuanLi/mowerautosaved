function [pos,vel,eul,stateVec_updated,P,a_n]=ekf9_tu(acc,pos_prev,vel_prev,quat,quat_prev,dt,P,Q,tau_acc)
%% 地球常数
axis_a = 6378137;             %赤道处地球半径[m]

%% 重力及科里奥利力补偿
g_n  = [0,0,9.8005]';

%% 位置与速度解算
Cnb = quat2dcm(quat)';
a_n = Cnb*acc + g_n  ;
vel = vel_prev + a_n *dt;
%梯形公式计算位置
pos(1) = pos_prev(1) + dt*0.5*(vel(1) + vel_prev(1));
pos(2) = pos_prev(2) + dt*0.5*(vel(2) + vel_prev(2));
pos(3) = pos_prev(3) + dt*0.5*(vel(3) + vel_prev(3));

%% Kalman filter time update
%denote
Z3 = zeros(3,3);    
I3 = eye(3);

%状态转移矩阵
g_mag = norm(g_n);                          %   Local gravity magnitude    
% dp2dp = -skew(wn_en);                       %   Pos. to pos. error block
dp2dv = diag((g_mag/axis_a)*[-1 -1 2]);        %   Pos. to vel. error block 
% dv2dv = -skew(2*wn_ie + wn_en);             %   Vel. to vel. error block
da2dv = Cnb ;                               %   Accel. bias to vel. error block                        
da2da = -I3./tau_acc;                        %   Accl. bias to accel bias error block
% da2da = eye();                        %   Accl. bias to accel bias error block

% % % general formula
% phi = [   dp2dp      I3          Z3    ;...
%           dp2dv      dv2dv       da2dv ;...
%           Z3         Z3          da2da ];

% for small dt
phi = [   Z3      I3       Z3    ;...
          dp2dv   Z3       da2dv ;...
          Z3      Z3       da2da ];

% %状态转移矩阵离散化
% FDisc = eye(length(phi(1,:))) + phi*dt + ((phi*dt)^2)/2;
% % FDisc = phi;
% 
% % G：系统噪声加权矩阵
% G = [ Z3  Z3  ;...
%       Cnb Z3  ;...
%       Z3  I3  ];
% % G = [ Cnb  Z3  ;...
% %       Z3 Cnb  ;...
% %       Z3  Z3  ];

Z = zeros(length(P));
Z(4,4) = dt;
Z(5,5) = dt;
Z(6,6) = dt;
FDisc = eye(length(P)) + Z;
G = zeros(9,3);
G(1:3,1:3) = 0.5*dt*dt*Cnb;
G(4:6,1:3) = dt*Cnb;
%计算离散Q矩阵
% QDisc = 0.5 *dt*(FDisc * G * Q * G'* FDisc'+ G * Q * G');
QDisc = G * Q * G';

%P矩阵计算
P = FDisc * P *(FDisc') + QDisc;
 
%保证P矩阵对称性,防止因为病态矩阵引起滤波器发散
P = 0.5*(P + transpose(P));

%保证对角线元素为正
for i=1:9
    if P(i,i) < 0
        P(i,i) = 0;
    end
end

eul = quat2eul1(quat);
eul_prev = quat2eul1(quat_prev);

z(4:6) = a_n*dt;   
z(1:3) = dt*0.5*(vel(1:3) + vel_prev(1:3));
z(7:9) = eul - eul_prev;
% stateVec_updated = F * stateVec;
stateVec_updated = z;

end