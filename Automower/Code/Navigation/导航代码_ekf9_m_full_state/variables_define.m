pos_gps_lla = [gps_lat_rad'; gps_lon_rad'; gps_height_m'];
gps_lla_ref = [gps_lat_rad,gps_lon_rad,gps_height_m]';
vel_gps_ned = zeros(3,length(pos_gps_lla));
acc_measurement_m_s2 = acc_measurement_norm*GRAV;

len = length(acc_measurement_m_s2) ;
acc_bias = [0,0,0]';
pos = zeros(3,len-1);
vel = zeros(3,len-1);
eul = zeros(3,len-1);
delta_odo_r = randn(1,len-1)*0.01;
delta_odo_l = randn(1,len-1)*0.01;

stateVec = zeros(9,len-1);           %×´Ì¬ÏòÁ¿
pos_prev = [0,0,0]';
vel_prev = [0,0,0]';
quat_prev = quat(1,:);