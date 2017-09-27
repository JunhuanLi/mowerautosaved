%note:quaternion in Hamilton form, i.e., q = [qw,qv]; full state:  state vector = q
len = length(imu_mx);
stateVec = zeros(4,len);
Hx = zeros(1,len);   %mag1
Hy = zeros(1,len);
Hx2 = zeros(1,len);   %mag2
Hy2 = zeros(1,len);
yaw_mag = zeros(1,len);
yaw_mag2 = zeros(1,len);
eul = zeros(3,len);

%coodinate frame alignment
GRAV = 9.8005;
acc_imu_m_s2 = [imu_ax_g -imu_ay_g -imu_az_g]'*GRAV;          %[m/s2]
angular_imu_rad_s = [imu_gx_rps -imu_gy_rps -imu_gz_rps]';   %[rad/s]
body_mx = mag_body(1,:);
body_my = mag_body(2,:);
body_mz = mag_body(3,:);
body_mx2 = mag_body2(1,:);
body_my2 = mag_body2(2,:);
body_mz2 = mag_body2(3,:);
clear imu_ax_g imu_ay_g imu_az_g imu_gx_rps imu_gy_rps imu_gz_rps