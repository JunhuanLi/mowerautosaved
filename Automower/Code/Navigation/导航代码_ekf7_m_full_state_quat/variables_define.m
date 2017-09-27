pos_gps_lla = [gps_lat_rad'; gps_lon_rad'; gps_height_m'];
for k=1:length(pos_gps_lla)
    if gps_state(k)>=2
        n_valid = k;
        break;
    end
end
gps_lla_ref = [gps_lat_rad(n_valid),gps_lon_rad(n_valid),gps_height_m(n_valid)]';
vel_gps_ned = [gps_vn_m_s,gps_ve_m_s,gps_vd_m_s]';
acc_measurement_m_s2 = acc_measurement_norm*GRAV;

len = length(acc_measurement_m_s2) ;
pos = zeros(3,len-1);
vel = zeros(3,len-1);
yaw = zeros(1,len-1);
delta_odo_r = zeros(1,len-1);
delta_odo_l = zeros(1,len-1);
stateVec = zeros(7,len-1);           %×´Ì¬ÏòÁ¿

for i=2:len
    delta_odo_r(i-1) = encoder_right_m(i) - encoder_right_m(i-1);
    delta_odo_l(i-1) = encoder_left_m(i) - encoder_left_m(i-1);
end

yaw(1) = yaw_att(2);
