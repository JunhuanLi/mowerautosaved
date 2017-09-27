function ned = wgsxyz2ned(p_e,ref_lat,ref_lon,ref_alt)

%   Calculate the relative position vector

p_e_ref = wgslla2xyz(ref_lat, ref_lon, ref_alt);
delta_xyz = p_e - p_e_ref;

%   Calculate ENU coordinates
enu = zeros(3,1);        
enu(1,1)= -sin(ref_lon)*delta_xyz(1) + cos(ref_lon)*delta_xyz(2);

enu(2,1)= -sin(ref_lat)*cos(ref_lon)*delta_xyz(1) - ...
            sin(ref_lat)*sin(ref_lon)*delta_xyz(2) + ...
            cos(ref_lat)*delta_xyz(3);
        
enu(3,1)= cos(ref_lat)*cos(ref_lon)*delta_xyz(1) + ...
            cos(ref_lat)*sin(ref_lon)*delta_xyz(2) + ... 
            sin(ref_lat)*delta_xyz(3);      
        
%   Define ENU to NED rotation matrix

C = [0 1 0;1 0 0;0 0 -1];

%   From NED position vector

ned = C*enu;
%===========================================================%     
