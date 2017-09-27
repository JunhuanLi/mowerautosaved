function cartPoints=spher2Cart(points)
%%SPHER2CART  Convert points from spherical coordinates to Cartesian
%             coordinates
%
%INPUTS:  points  One or more points given in terms of range, azimuth and
%                 elevation, with the angles in radians. To convert
%                 N points, points is a 3XN matrix with each column
%                 having the format [range;azimuth; elevation].
%
%OUTPUTS:   cartPoints For N points, cartPoints is a 3XN matrix of the
%                      converted points with each column having the format
%                      [x;y;z].

%Extract the coordinates
r=points(1,:);
azimuth=points(2,:);
elevation=points(3,:);

x=r.*cos(azimuth).*cos(elevation);
y=r.*sin(azimuth).*cos(elevation);
z=r.*sin(elevation);

cartPoints=[x;y;z];

end
