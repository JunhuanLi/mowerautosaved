clear;clc;close all
%poseVec = [x,y,theta,vx,vy,omega_theta]
%stateVec = delta_poseVec
%theta: angle between +x and north, counterclockwise positive
len = 1000;
poseVec = zeros(6,len);
stateVec = zeros(6,len);

for x=1:100
%% 参数设定
P = eye(6);
D = 0.75;    %vehile diamter[m]
dt = 0.1;

R_imu = diag([0.01,0.01,0.01,0.01,0.01,0.01]*.1.^2);
%method 1
R = diag([4e-3,0.0023,2e-3]);
Q = diag(ones(1,6)*0.01);  

%method 2
Q_odom = diag([4e-3,0.0023])*1;   %corresponding to l and r

%% 模拟数据产生
ax = randn(1,len)*0.01;   %body frame x
ay = randn(1,len)*0.01;   %body frame y
w = randn(1,len)*0.01;
delta_odo_r = ones(1,len)*0.1+randn(1,len)*0.01/5;
delta_odo_l = ones(1,len)*0.1+randn(1,len)*0.01/5;
mag = ones(1,len)*pi/4+randn*0.01;

%初始速度方向设定 
poseVec(4,1) = 1;           %初始x速度
poseVec(5,1) = 0;           %初始y速度
poseVec(3,1) = 0;          %初始theta
delta_theta = poseVec(3,1);
stateVec(:,1) = [0.1,0,0,0,0,0]';

% ground truth
ground_truth_x = (1:len)*dt; 
ground_truth_y = (1:len)*dt*0;
vel_truth_x = poseVec(4,1)*ones(1,len)*1;
vel_truth_y = poseVec(5,1)*ones(1,len)*0;

%% loop
for i=2:len
    %% time update
%     [stateVec(:,i),poseVec(:,i),P]=imu_tu(poseVec(:,i-1),stateVec(:,i-1),ax(i),ay(i),w(i),P,dt,Q);

     %method 1
%      P = P + Q;
%      stateVec(:,i) = stateVec(:,i-1);
    
%     %method 2
    [stateVec(:,i),poseVec(:,i),P,delta_theta]=odom_tu(poseVec(:,i-1),stateVec(:,i-1),delta_odo_r(i),delta_odo_r(i-1),delta_odo_l(i),delta_odo_l(i-1),P,Q_odom,D,delta_theta);
    
    %% odometry update
    %method 1
    [stateVec(:,i),poseVec(:,i),P,z(:,i)]=odometry_mu(poseVec(:,i-1),stateVec(:,i),delta_odo_r(i),delta_odo_l(i),P,R,D);

    %% imu measurement update
%     [stateVec(:,i),poseVec(:,i),P,var_z(i,:)]=imu_mu(poseVec(:,i-1),stateVec(:,i),ax(i),ay(i),w(i),P,R_imu,dt);
%     [stateVec_updated,P_updated,var_z]=imu_mu(poseVec_prev,stateVec,axb,ayb,w,P,R,dt)

    %% magnetic update
    
    %% pose vector update
%     poseVec(:,i) = poseVec(:,i-1) + stateVec(:,i);
    norm_P(i) = norm(P);
end

% % %mean square evaluation
msqr_x(x) = meansqr(poseVec(1,:)-ground_truth_x);
msqr_y(x) = meansqr(poseVec(2,:)-ground_truth_y);
end
figure_output;
mean(msqr_x)
mean(msqr_y)