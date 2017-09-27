pos_gps_lla = [gps_lat_rad'; gps_lon_rad'; gps_height_m'];
gps_lla_ref = [gps_lat_rad,gps_lon_rad,gps_height_m]';
vel_gps_ned = zeros(3,length(pos_gps_lla));
acc_measurement_m_s2 = acc_measurement_norm*GRAV;

% stateVec = zeros(12,length(acc_measurement_m_s2));                                                                            
% stateVec(10:12,1) = -acc_measurement_m_s2(:,1); 
% delta_stateVec = zeros(12,1);
len = length(acc_measurement_m_s2) - head;
acc_bias = [0,0,0]';
pos = zeros(3,len-1);
vel = zeros(3,len-1);
stateVec = zeros(9,len-1);           %×´Ì¬ÏòÁ¿

pos_prev = [0,0,0]';
vel_prev = [0,0,0]';
quat_prev = quat(1,:);