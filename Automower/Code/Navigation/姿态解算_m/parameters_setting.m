sigma_an = [0.1,0.1,0.1]*.05;         %m/s2
sigma_wn = [0.05,0.05,0.05]*.05;      %rad/s
%sigma_aw = [0.1,0.1,0.1]*1;      %m/s2/sqrt(s)
%sigma_ww = [0.01,0.01,0.01]*1;   %rad/s/sqrt(s)

Q = diag(sigma_wn.^2)*1;
R = diag(sigma_an.^2)*20;
P = eye(size(stateVec,1))*10;
delta_heading_rad = -0.0421;     %declination angle