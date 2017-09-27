%% remove head invalid values 
head = 10;
imu_ax_g = imu_ax_g(head:end);
imu_ay_g = imu_ay_g(head:end);
imu_az_g = imu_az_g(head:end);
imu_gx_rps = imu_gx_rps(head:end);
imu_gy_rps = imu_gy_rps(head:end);
imu_gz_rps = imu_gz_rps(head:end);
imu_mx = imu_mx(head:end);
imu_my = imu_my(head:end);
imu_mz = imu_mz(head:end);
imu_mx2 = imu_mx2(head:end);
imu_my2 = imu_my2(head:end);
imu_mz2 = imu_mz2(head:end);

%% outliers removal and mediate filter
imu_ax_g2 = hampelf(imu_ax_g);
imu_ax_g = hampelf(imu_ax_g);
imu_ay_g = hampelf(imu_ay_g);
imu_az_g = hampelf(imu_az_g);
imu_az_g2 = hampelf(imu_az_g);

imu_gx_rps = hampelf(imu_gx_rps);
imu_gy_rps = hampelf(imu_gy_rps);
imu_gz_rps = hampelf(imu_gz_rps);
imu_mx = hampelf(imu_mx);
imu_my = hampelf(imu_my);
imu_mz = hampelf(imu_mz);
imu_mx2 = hampelf(imu_mx2);
imu_my2 = hampelf(imu_my2);
imu_mz2 = hampelf(imu_mz2);
gps_heading_deg = hampelf(gps_heading_deg);
