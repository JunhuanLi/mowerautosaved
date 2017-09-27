function cartPoints=ellips2Cart(points,a,f)
%#codgen
%%ELLIPS2CART Convert ellipsoidal coordinates to ECEF Cartesian
%             coordinates.
%
%INPUTS:    points  One or more points given in geodetic latitude and
%                   longitude, in radians, and height, in meters that are
%                   to be converted to Cartesian coordinates. To convert
%                   N points, points is a 3XN matrix with each column
%                   having the format [latitude;longitude; height].
%           a       The semi-major axis of the reference ellipsoid. If
%                   this argument is omitted, the value in
%                   Constants.WGS84SemiMajorAxis is used.
%           f       The flattening factor of the reference ellipsoid. If
%                   this argument is omitted, the value in
%                   Constants.WGS84Flattening is used.
%
%OUTPUTS:   cartPoints For N points, cartPoints is a 3XN matrix of the
%                      converted points with each column having the format
%                      [x;y;z].
%
%The conversions are mentioned in the paper "Simulating Aerial Targets in
%3D Accounting for the Earth's Curvature" by David F. Crouse.
%
%September 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.
%(UNCLASSIFIED) DISTRIBUTION STATEMENT A. Approved for public release.

% f = 1/298.257223563;    %flattening
% a = 6378136.3;          %seminmajor [m]

%The geodetic latitudes
phi=points(1,:);
%The longitudes
lambda=points(2,:);
%The altitudes
h=points(3,:);

sinP=sin(phi);
cosP=cos(phi);
sinL=sin(lambda);
cosL=cos(lambda);

%The square of the first numerical eccentricity
e2=2*f-f^2;
%The normal radii of curvature.
Ne=a./sqrt(1-e2*sinP.^2);

x=(Ne+h).*cosP.*cosL;
y=(Ne+h).*cosP.*sinL;
z=(Ne*(1-e2)+h).*sinP;

cartPoints=[x;y;z];
end
