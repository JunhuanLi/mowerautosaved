clc;clear;
close all;
%% load file
%by name
% file = '0705_2_roof';
% suffix = '.mat';
% file_name = [file,suffix];
% load(file_name);

%by ui interface
[file_name, pathname] = uigetfile('*.mat', '读取数据文件','C:\Users\Administrator\Desktop\Data file'); 
load(file_name);
file_name(end-3:end)=[];

%% Preprocessing: invalid data and outlier remove
head = 10;
if(1)
imu_mx = imu_mx(head:end);
imu_my = imu_my(head:end);
imu_mz = imu_mz(head:end);
imu_mx2 = imu_mx2(head:end);
imu_my2 = imu_my2(head:end);
imu_mz2 = imu_mz2(head:end);

else
imu_mx = imu_mx(head:1000);
imu_my = imu_my(head:1000);
imu_mx2 = imu_mx2(head:1000);
imu_my2 = imu_my2(head:1000);

end

%% Ellipse fitting on mag output and mapping to unit cicle
ellipse_t = fit_ellipse(imu_mx,imu_my);
ellipse_t2 = fit_ellipse(imu_mx2,imu_my2);

mag_body = mag_fitting_ellipse(ellipse_t,imu_mx,imu_my,imu_mz);
mag_body2 = mag_fitting_ellipse(ellipse_t2,imu_mx2,imu_my2,imu_mz2);

% figure;plot(mag_body(1,:),mag_body(2,:));grid on;hold on;plot(mag_body2(1,:),mag_body2(2,:));
% legend('mag1','mag2');axis equal

%% Analysis in body frame
%parameters calculation for analysis
% yaw = (-atan2(mag_body(2,:),mag_body(1,:)) + ellipse_t.phi)*180/pi;
% yaw2 = (-atan2(mag_body2(2,:),mag_body2(1,:)) + ellipse_t2.phi)*180/pi;
yaw = -atan2(mag_body(2,:),mag_body(1,:))*180/pi;
yaw2 = -atan2(mag_body2(2,:),mag_body2(1,:))*180/pi;
yaw2 = yaw2+(yaw(1)-yaw2(1));

dif_mx = mag_body(1,:)-mag_body2(1,:);
dif_my = mag_body(2,:)-mag_body2(2,:);
delta_yaw = mod(yaw - yaw2,360);

%range shift
for k =1:length(yaw)
    if(abs(delta_yaw(k)-360)<20)
        delta_yaw(k) = delta_yaw(k) - 360;
    end
    if (yaw(k)<0)&&(yaw(k)>-180)
        yaw(k) = yaw(k) + 360;
    end
    if (yaw2(k)<0)&&(yaw2(k)>-180)
        yaw2(k) = yaw2(k) + 360;
    end
end

%% delta yaw fitting and periodic signal removal
ft = fittype( @(a, b, c, d,e,x) a+b*sin(x)+c*sin(2*x)+d*cos(x)+e*cos(2*x));
x = yaw/57.3;
y = delta_yaw/57.3;
% Fit the curve to the data
f = fit( x',y',ft);
Residual = f.b*sin(x)+f.c*sin(2*x)+f.d*cos(x)+f.e*cos(2*x);
delta_yaw2 = delta_yaw - Residual*57.3;
% figure;plot(delta_yaw);hold on;plot(delta_yaw2);grid on;

% figure;plot(mag_body(1,:));hold on;plot(mag_body2(1,:));grid on;
% legend('mag1','mag2');title('mx in body frame');xlabel('Samples')

% figure;plot(mag_body(2,:));hold on;plot(mag_body2(2,:));
% grid on;legend('mag1','mag2');title('my in body frame');xlabel('Samples')

% figure;plot(dif_mx,dif_my);xlabel('mx difference');ylabel('my difference');grid on
% figure;plot(dif_mx);hold on;plot(dif_my);hold on;

figure;
subplot(211);plot(delta_yaw2);grid on;ylabel('Delta Yaw Angle [deg]');xlabel('Samples');title('Yaw vs. Delta Yaw')
subplot(212);plot(yaw,'r');grid on;ylabel('Yaw Angle [deg]');xlabel('Samples')
figure;plot(yaw);hold on;plot(yaw2);grid on;legend('mag1','mag2');title('Yaw angle [deg]')

%% save ellipse and curve fitting model to environment files
save(['C:\Users\Administrator\Desktop\Data file\', 'environment file.mat'],'ellipse_t','ellipse_t2','f');
