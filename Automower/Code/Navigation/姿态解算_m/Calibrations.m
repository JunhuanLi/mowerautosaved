%% magnetometer elipse calibration
load('environment file.mat');
mag_body = mag_fitting_ellipse(ellipse_t,imu_mx,imu_my,imu_mz);
mag_body2 = mag_fitting_ellipse(ellipse_t2,imu_mx2,imu_my2,imu_mz2);

%% gyro calibration;
cali_time = 60;
cali_len = cali_time/dt;
imu_gx_rps = imu_gx_rps - mean(imu_gx_rps(1:cali_len));                                          
imu_gy_rps = imu_gy_rps - mean(imu_gy_rps(1:cali_len));
imu_gz_rps = imu_gz_rps - mean(imu_gz_rps(1:cali_len));

%% accelerometer cali