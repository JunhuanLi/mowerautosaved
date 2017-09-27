% % %% PART1: Base location
% clear;clc;close all;
% data = load('D:\Automower\Code\Navigation\pvt解帧和基站识别\rtk_simplified\center_out.txt');
% lon_rad = hampel(data(:,1)*pi/180);
% lat_rad = hampel(data(:,2)*pi/180);
% hMSL_m = hampel(data(:,3));
% height_m = hampel(data(:,4));
% hAcc = hampel(data(:,5));
% gps_pos_lla = [lat_rad,lon_rad,hMSL_m];
% % pos_lla_ref = [0.395671796293891,113.917776724020/57.3,57.6286865987097];
% sum = zeros(1,3);
% iter = 0;
% len = length(gps_pos_lla);
% clear data
% %% 
% for k=300:len
%     if hAcc(k)<5
%         sum = sum + gps_pos_lla(k,:);
%         iter = iter + 1;
%     end
% end
% pos_lla_ref = sum/iter;
% figure;plot(gps_pos_lla(:,1),'*');grid on;%xlabel('N directiohn [m]');ylabel('E directiohn [m]')
% hold on;plot(pos_lla_ref(1),'o')
% figure;plot(gps_pos_lla(:,2),'*');grid on;%xlabel('N directiohn [m]');ylabel('E directiohn [m]')
% hold on;plot(pos_lla_ref(2),'o')

%%
%% PART2: Analysis
clear;clc;close all;
data = load('D:\Automower\Code\Navigation\pvt解帧和基站识别\rtk_simplified\pvt_out.txt');
lon_rad = hampel(data(:,1)*pi/180);
lat_rad = hampel(data(:,2)*pi/180);
hMSL_m = hampel(data(:,3));
height_m = hampel(data(:,4));
hAcc = hampel(data(:,5));
gps_pos_lla = [lat_rad,lon_rad,hMSL_m];
pos_lla_ref = [0.395671946406873,1.98809380030523,57.4655251701681];
len = length(gps_pos_lla);
clear data
%% 
for k=1:len
    if hAcc(k)<5
        gps_pos_ecef = wgslla2xyz(gps_pos_lla(k,1),gps_pos_lla(k,2),gps_pos_lla(k,3));
        gps_pos_ned(:,k) = wgsxyz2ned(gps_pos_ecef,pos_lla_ref(1),pos_lla_ref(2),pos_lla_ref(3));
    end
end

figure;plot(gps_pos_ned(1,:),gps_pos_ned(2,:),'*');grid on;xlabel('N directiohn [m]');ylabel('E directiohn [m]')
hold on;rectangle('Position',[-2,-2,4,4],'Curvature',[1,1]),axis equal;title('GPS points');
hold on;plot(0,0,'O')