%simplified IMU model
clear;close all;clc

%data input
%data preprocessing

%settings
%variables definition:
%p_position,
%v_velocity,
%q_quaternion,
%theta:rotation angle vector
%am_acceleration measurement,an_acceleration noise
%wm_angular rate measurement,wn_angular rate noise
%R_rotation matrix

%w = wm - wb
%delta_w = -delta_wb - wn (wt = w + delta_w)
%full state:  x = [delta_p,delta_v,delta_theta];

%initialization
x = zeros(9,1);  

%time udpate
%delta state update
x_updated = A*x;
Z = zeros(3,3);
P_v = eye(3);
V_theta = -R*cross3(a);
psi_theta = -cross3()
A = [Z P_v Z;Z Z V_theta;Z Z psi_theta];

P_updated = Fx * P * Fx' + Fi * Q * Fi';
%true state update


%measurement update