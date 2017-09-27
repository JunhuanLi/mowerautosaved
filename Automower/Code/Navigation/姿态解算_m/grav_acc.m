function g = grav_acc(lat_rad)
%#codegen
g45 = 9.806;         %[m/s2]
g_poles = 9.832;     %[m/s2]
g_equ = 9.780;       %[m/s2]
g = g45 - 0.5*(g_poles-g_equ)*cos(2*lat_rad);
end