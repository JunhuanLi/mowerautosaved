clear;clc;close all
%poseVec = [x,y,theta,vx,vy,omega_theta]
%stateVec = delta_poseVec
%theta: angle between +x and north, counterclockwise positive
len = 1000;
for x=1:1
poseVec = zeros(6,len);
stateVec = zeros(6,len);
%% 参数设定
P = eye(6);
D = 0.75;    %vehicle diamter[m]
dt = 0.1;
Q = diag([0.01,0.01,0.01].^2);
R = diag([4e-3,0.0023,2e-2])*1000;

%% 模拟数据产生
ax = randn(1,len)*0.01;   %body frame x
ay = randn(1,len)*0.01;   %body frame y
w = randn(1,len)*0.01;
delta_odo_r = ones(1,len)*0.1+randn(1,len)*0.001;
delta_odo_l = ones(1,len)*0.1+randn(1,len)*0.001;
mag = ones(1,len)*pi/4+randn*0.01;

%初始速度方向设定 
poseVec(4,1) = 1;          %初始x速度
poseVec(5,1) = 0;          %初始y速度
poseVec(3,1) = 0;          %初始theta

% ground truth
ground_truth_x = (1:len)*poseVec(4,1)*dt; 
ground_truth_y = (1:len)*poseVec(5,1)*dt;
vel_truth_x = poseVec(4,1)*ones(1,len);
vel_truth_y = poseVec(5,1)*ones(1,len);

%% loop
for i=2:len
    %% time update
    [stateVec(:,i),poseVec(:,i),P] = imu_tu(poseVec(:,i-1),stateVec(:,i-1),ax(i),ay(i),w(i),P,dt,Q);

    %% odometry update
    [stateVec(:,i),poseVec(:,i),P,z(:,i)] = odometry_mu(poseVec(:,i-1),poseVec(:,i),stateVec(:,i),delta_odo_r(i),delta_odo_l(i),P,R,D,dt);
    
    %% odometry vel update
%     [stateVec(:,i),poseVec(:,i),P]=odometry_mu_vel(poseVec(:,i-1),stateVec(:,i),delta_odo_r(i),delta_odo_l(i),P,R,D,dt);

    %% magnetic update
    
    %% pose vector update
    norm_P(i) = norm(P);
    
end

% %mean square evaluation
msqr_x(x) = meansqr(poseVec(1,:)-ground_truth_x)/len;
msqr_y(x) = meansqr(poseVec(2,:)-ground_truth_y)/len;
end
figure_output;
mean(msqr_x)
mean(msqr_y)