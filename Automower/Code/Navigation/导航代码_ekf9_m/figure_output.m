%% gps position out
%%%%method 1 by own functions, faster
for k = valid_gps_flag:len
pos_gps_ecef(:,k) = wgslla2xyz(pos_gps_lla(1,k),pos_gps_lla(2,k),pos_gps_lla(3,k));
pos_gps_ned(:,k) = wgsxyz2ned(pos_gps_ecef(:,k),gps_lla_ref(1),gps_lla_ref(2),gps_lla_ref(3));
end

%%%%%method 2 by matlab functions,slower
% pos_gps_lla_ddm = [pos_gps_lla(1,:)*57.3;pos_gps_lla(2,:)*57.3;pos_gps_lla(3,:)];
% for k = valid_gps_flag:len
% flatearth_pos(:,k) = lla2flat(pos_gps_lla_ddm(:,k)', gps_lla_ref(1:2)*57.3, 0, gps_lla_ref(3))';
% end
% figure;
% subplot(211);plot(flatearth_pos(1,:));
% subplot(212);plot(flatearth_pos(2,:));

%% position output
figure;
subplot(211);plot((1:len-1)*parameters.dt,pos(1,:));xlabel('time [s]');ylabel('x position');grid on;hold on;plot((1:len)*parameters.dt,pos_gps_ned(1,:));legend('EKF','GPS');
subplot(212);plot((1:len-1)*parameters.dt,pos(2,:));xlabel('time [s]');ylabel('y position');grid on;hold on;plot((1:len)*parameters.dt,pos_gps_ned(2,:));

%% horizontal movement
% figure;
% % plot((1:length(acc_measurement))*parameters.dt,stateVec(1,:));
% plot(pos(1,:),pos(2,:));
% xlabel('stateVec position');ylabel('y position');legend('planery posistion');grid on;

%% acceleration bias output
figure;
subplot(311);plot((1:len-1)*parameters.dt,stateVec(7,:));xlabel('time [s]');ylabel('ax\_bias');grid on;title('acceleration bias')
subplot(312);plot((1:len-1)*parameters.dt,stateVec(8,:));xlabel('time [s]');ylabel('ay\_bias');grid on;
subplot(313);plot((1:len-1)*parameters.dt,stateVec(9,:));xlabel('time [s]');ylabel('az\_bias');grid on;

%% velocity output
figure;
subplot(211);plot((1:len-1)*parameters.dt,vel(1,:));xlabel('time [s]');ylabel('vx');grid on;title('velocity [m/s]')
subplot(212);plot((1:len-1)*parameters.dt,vel(2,:));xlabel('time [s]');ylabel('vy');grid on;
% subplot(313);plot((1:len)*parameters.dt,stateVec(6,:));xlabel('time [s]');ylabel('vz');grid on;

% % %% euler angle output
% % figure;
% % subplot(311);plot((1:len)*parameters.dt,eul(1,1:len)*180/pi);hold on;plot((1:len)*parameters.dt,att_roll_rad(1:len)*180/pi);xlabel('time [s]');ylabel('roll angle [deg]');grid on;title('Euler Angle');legend('Calculation','EVK output');
% % subplot(312);plot((1:len)*parameters.dt,eul(2,1:len)*180/pi);hold on;plot((1:len)*parameters.dt,att_pitch_rad(1:len)*180/pi);xlabel('time [s]');ylabel('pitch angle [deg]');grid on;
% % subplot(313);plot((1:len)*parameters.dt,eul(3,1:len)*180/pi);hold on;plot((1:len)*parameters.dt,att_yaw_rad(1:len)*180/pi);xlabel('time [s]');ylabel('yaw angle [deg]');grid on;
% 
%% norm P 
figure;
plot(norm_P);grid on;title('norm P');

%% Kalman gain
% figure;
% plot(K_norm);grid on;title('Kalman gain');

% %% gravity output
% for k = 1:len
% g_norm(k) = norm(stateVec(10:12,k));
% end
% figure;plot(g_norm);grid on;title('Calculated Gravity Value [m/s2] ');
% 
% %% DOPs
% % figure;plot(gps_HDOP(1:len));hold on;plot(gps_VDOP(1:len));hold on;plot(gps_PDOP(1:len));legend('HDOP','VDOP','PDOP');grid on
