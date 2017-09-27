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
