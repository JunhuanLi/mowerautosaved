load untitled.mat;
a = ans.Data;
for i=1:size(a,3)
    y = decode_stm32(ans(1,:,i));
    gps_lat_rad(i)=y(1);
    gps_lon_rad(i)=y(2);
    gps_height_m(i)=y(3);
    gps_state(i)=y(3);
end


y(1) = double(typecast(uint8([u(5),u(4),u(3),u(2)]), 'uint32'))/10000000*pi/180;    %latitude [rad]
y(2) = double(typecast(uint8([u(9),u(8),u(7),u(6)]), 'uint32'))/10000000*pi/180;    %longitude [rad]
y(3) = u(10);       %定位类型，1：没有定位，2：2D定位，3：3D定位
y(4) = double(typecast(uint8([u(14),u(13),u(12),u(11)]), 'uint32'))*0.001;          %height [m]
y(5) = double(typecast(uint8([u(16), u(15)]), 'uint16'))*0.001;                     %hAcc[m]
y(6) = double(typecast(uint8([u(18), u(17)]), 'uint16'))*0.001;                     %vAcc[m]
y(7) = double(typecast(uint8([u(20), u(19)]), 'uint16'))*0.001;                     %sAcc[m/s]
y(8) = double(typecast(uint8([u(24),u(23),u(22),u(21)]), 'uint32'))*0.001;          %vn [m/s]
y(9) = double(typecast(uint8([u(28),u(27),u(26),u(25)]), 'uint32'))*0.001;          %ve [m/s]
y(10) = double(typecast(uint8([u(32),u(31),u(30),u(29)]), 'uint32'))*0.001;         %vd [m/s]
y(11) = double(typecast(uint8([u(34), u(33)]), 'int16'))/16384;                     % accx [g]
y(12) = double(typecast(uint8([u(36), u(35)]), 'int16'))/16384;                     % accy [g]
y(13) = double(typecast(uint8([u(38), u(37)]), 'int16'))/16384;                     % accz [g]
y(14) = double(typecast(uint8([u(40), u(39)]), 'int16'))/16.4*pi/180;               % gx [rad/s]
y(15) = double(typecast(uint8([u(42), u(41)]), 'int16'))/16.4*pi/180;               % gy [rad/s]
y(16) = double(typecast(uint8([u(44), u(43)]), 'int16'))/16.4*pi/180;               % gz [rad/s]
y(17) = double(typecast(uint8([u(46), u(45)]), 'int16'));                           % mx
y(18) = double(typecast(uint8([u(48), u(47)]), 'int16'));                           % my
y(19) = double(typecast(uint8([u(50), u(49)]), 'int16'));                           % mz
y(20) = double(typecast(uint8([u(52), u(51)]), 'int16'))*0.01*pi/180;               % roll [rad]
y(21) = double(typecast(uint8([u(54), u(53)]), 'int16'))*0.01*pi/180;               % pitch [rad]
y(22) = double(typecast(uint8([u(56), u(55)]), 'int16'))*0.01*pi/180;               % yaw [rad]  
y(23) = u(57);                                                                      % valid SV