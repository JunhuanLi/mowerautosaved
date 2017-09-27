%initial phase differece elimination
yaw_mag2 = yaw_mag2+(yaw_mag(2)-yaw_mag2(2));
% yaw_mag2 = yaw_mag2-(ellipse_t.phi-ellipse_t2.phi);
yaw_mag2 = mod(yaw_mag2,2*pi);

%delta yaw angle comparison
dif_mx = Hx-Hx2;
dif_my = Hy-Hy2;
delta_yaw = mod(yaw_mag - yaw_mag2,2*pi);

figure;plot(yaw_mag*180/pi);hold on;plot(yaw_mag2*180/pi);grid on;legend('mag1','mag2');
ylabel('Yaw angle [deg]')

for k =1:len
    if(abs(delta_yaw(k)*57.3-360)<30)
        delta_yaw(k) = delta_yaw(k) - 2*pi;
    end
end

%delta yaw fitting and remove periodic signal part
x = yaw_mag;
Residual = f.b*sin(x)+f.c*sin(2*x)+f.d*cos(x)+f.e*cos(2*x);
delta_yaw2 = delta_yaw - Residual;

figure;plot(delta_yaw(2:end)*180/pi);grid on;ylabel('Delta Yaw Angle [deg]');xlabel('Samples');title('Yaw vs. Delta Yaw')
hold on;plot(delta_yaw2(2:end)*180/pi);legend('Origianl','Sine sig removed')