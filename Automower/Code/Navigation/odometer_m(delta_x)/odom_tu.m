function [stateVec_updated,poseVec_updated,P_updated,delta_theta_updated]=odom_tu(poseVec,stateVec,r,r_prev,l,l_prev,P,Q,D,delta_theta)
%Pk = [x,y,theta]
%Xk = Pk-Pk_1;
%%
A = (l+r)/2;
B = (l_prev+r_prev)/2;

theta = poseVec(3);
delta_theta_updated = (l-r)/D;
alpha = delta_theta_updated/2;
beta = delta_theta/2;
theta_k_k_1=stateVec(3);

Z = zeros(6);
Z(1,3) = B*cos(theta+alpha)*sin(theta_k_k_1+alpha-beta)+B*cos(theta_k_k_1+alpha-beta)*sin(theta+alpha);
Z(2,3) = B*sin(theta+alpha)*sin(theta_k_k_1+alpha-beta)-B*cos(theta_k_k_1+alpha-beta)*cos(theta+alpha);
FDisc = eye(6) + Z;
U = zeros(6,1);   %与stateVec_k_1无关的项
U(1) = A*cos(theta+alpha);
U(2) = A*sin(theta+alpha);
U(3) = delta_theta_updated - delta_theta;
V = zeros(6,1);
V(1) = -B*cos(theta+alpha)*cos(theta_k_k_1+alpha-beta)+B*sin(theta_k_k_1+alpha-beta)*sin(theta+alpha);
V(2) = -B*sin(theta+alpha)*cos(theta_k_k_1+alpha-beta)-B*sin(theta_k_k_1+alpha-beta)*cos(theta+alpha);
% stateVec_updated = FDisc*stateVec+U;
stateVec_updated = stateVec + V + U ;

%噪声传递矩阵
G = zeros(6,2);
G(1,1)= -cos(theta+alpha)*sin(theta_k_k_1+alpha-beta)+cos(theta_k_k_1+alpha-beta)*sin(theta+alpha);
G(1,2)= G(1,1);
G(2,1) = sin(theta+alpha)*sin(theta_k_k_1+alpha-beta)-cos(theta_k_k_1+alpha-beta)*cos(theta+alpha);
G(2,2) = G(2,1);

%计算离散Q矩阵
dt = 0.1;
% QDisc = 0.5 *dt*(FDisc * G * Q * G'* FDisc'+ G * Q * G');
QDisc = G * Q * G';

%P矩阵计算
P = FDisc * P *(FDisc') + QDisc;
 
%保证P矩阵对称性,防止因为病态矩阵引起滤波器发散
P_updated = 0.5*(P + transpose(P));

%保证对角线元素为正
for i=1:length(P)
    if P_updated(i,i) < 0
        P_updated(i,i) = 0;
    end
end

%pose vector time update
 poseVec_updated = poseVec + stateVec_updated;
%  stateVec_updated'
%  poseVec_updated'
end
