function p_e = wgslla2xyz(lat,lon,alt)

% ellipsoid constants
f = 1/298.257223563;        %   WGS-84 Flattening.
e = sqrt(f*(2 - f));        %   Eccentricity.
R_0 = 6378137;              %   WGS-84 equatorial radius (m).                            

%   Compute East-West Radius of curvature at current position

R_E = R_0/(sqrt(1 - (e*sin(lat))^2));

%   Compute ECEF coordinates
p_e = zeros(3,1);
p_e(1,1) = (R_E + alt)*cos(lat)*cos(lon);
p_e(2,1) = (R_E + alt)*cos(lat)*sin(lon);
p_e(3,1) = ((1 - e^2)*R_E + alt)*sin(lat);

%===========================================================%     
