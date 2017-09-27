% %ellipse fitting
% ellipse_t = fit_ellipse(imu_mx,imu_my);    %parameters

function mag_body = mag_calibration_ellipse(ellipse_t,imu_mx,imu_my,imu_mz)
%mapping to circle
phi = ellipse_t.phi;
B = [ellipse_t.X0;ellipse_t.Y0];
Xf = max(1,ellipse_t.b/ellipse_t.a);
Yf = max(1,ellipse_t.a/ellipse_t.b);
T = diag([Xf,Yf]);   
% T = diag([1,ellipse_t.a/ellipse_t.b]);
A = [cos(phi) -sin(phi);sin(phi) cos(phi)];

% mag_body = zeros(3,length(imu_mx));
for k = 1:length(imu_mx)
    mag_body(1:2,k) = T*(A * [imu_mx(k);imu_my(k)])-T*B;
    mag_body(3,k) = imu_mz(k);
end
% hold on
% plot(imu_mx,imu_my);
% hold on;plot(ellipse_t.X0*1.27,ellipse_t.Y0,'*')
end