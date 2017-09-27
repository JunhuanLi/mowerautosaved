%% remove head invalid values 
head = 10;
imu_ax_g = imu_ax_g(head:end);
imu_ay_g = imu_ay_g(head:end);
imu_az_g = imu_az_g(head:end);

gps_state = gps_state(head:end);
gps_lat_rad = gps_lat_rad(head:end);
gps_lon_rad = gps_lon_rad(head:end);
gps_height_m = gps_height_m(head:end);
gps_vn_m_s = gps_vn_m_s(head:end);
gps_ve_m_s = gps_ve_m_s(head:end);
gps_vd_m_s = gps_vd_m_s(head:end);
gps_vAcc_m = gps_vAcc_m(head:end);
gps_hAcc_m = gps_hAcc_m(head:end);
gps_sAcc_m_s = gps_sAcc_m_s(head:end);
gps_heading_deg = gps_heading_deg(head:end);
gps_headAcc_deg = gps_headAcc_deg(head:end);

%mediate value filter to remove outliers
%notice:gps altitude not accurate and with outliers appearance
% gps_altitude = medfilt1(gps_altitude,15);
% att_roll_rad = medfilt1(att_roll_rad,7);
% att_pitch_rad = medfilt1(att_pitch_rad,7);
% att_yaw_rad = medfilt1(att_yaw_rad,7);

%% hampel function to remove outliers
gps_hAcc_m = hampel(gps_hAcc_m);
gps_vAcc_m = hampel(gps_vAcc_m);
gps_sAcc_m_s = hampel(gps_sAcc_m_s);
gps_lat_rad = hampel(gps_lat_rad);
gps_lon_rad = hampel(gps_lon_rad);
gps_height_m = hampel(gps_height_m);
gps_vn_m_s = hampel(gps_vn_m_s);
gps_ve_m_s = hampel(gps_ve_m_s);
gps_vd_m_s = hampel(gps_vd_m_s);
imu_ax_g = hampel(imu_ax_g);
imu_ay_g = hampel(imu_ay_g);
imu_az_g = hampel(imu_az_g);
gps_heading_deg = hampel(gps_heading_deg,7);
gps_headAcc_deg = hampel(gps_headAcc_deg,7);
encoder_left_m = hampel(encoder_left_m,7);
encoder_right_m = hampel(encoder_right_m,7);

% gps_heading_deg = medfilt1(gps_heading_deg,23);
acc_measurement_norm = zeros(3,length(imu_ax_g));

% calibration;
for k = 1:length(imu_ax_g)
acc_measurement_norm(:,k) = [imu_ax_g(k) -imu_ay_g(k) -imu_az_g(k)]'/norm([imu_ax_g(k) imu_ay_g(k) imu_az_g(k)]);          %[m/s2]
end
clear imu_ax_g imu_ay_g imu_az_g imu_gx_rps imu_gy_rps imu_gz_rps

quat = quat';