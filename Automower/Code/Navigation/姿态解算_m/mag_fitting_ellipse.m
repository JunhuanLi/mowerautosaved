% %ellipse fitting
function mag_body = mag_fitting_ellipse(ellipse_t,imu_mx,imu_my,imu_mz)
%mapping to circle
phi = ellipse_t.phi;
B = [ellipse_t.X0;ellipse_t.Y0];
Xf = max(1,ellipse_t.b/ellipse_t.a);
Yf = max(1,ellipse_t.a/ellipse_t.b);
T = diag([Xf,Yf]);   
% T = diag([1,ellipse_t.a/ellipse_t.b]);
A = [cos(phi) -sin(phi);sin(phi) cos(phi)];

for k = 1:length(imu_mx)
    mag_body(1:2,k) = T*((A * [imu_mx(k);imu_my(k)]) - B);%/max(ellipse_t.a,ellipse_t.b);
    mag_body(3,k) = imu_mz(k);
end

% figure;plot(mag_body(1,:));hold on;plot(mag_body(2,:));grid on;legend('mx','my');
% figure;plot(mag_body(1,:),mag_body(2,:))
end
