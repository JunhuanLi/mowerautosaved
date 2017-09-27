function points=ellips2Sphere(points)
%#codegen
%%ELLIPS2SPHERE Convert ellipsoidal coordinates to spherical coordinates.
%               If the ellipsoidal coordinates are not full coordinates,
%               then it is assumed that one is just converting latitudes on
%               the surface of the reference ellipsoid.
%
%INPUTS:    points  One or more points given in geodetic latitude and
%                   longitude, in radians, and height, in meters that are
%                   to be converted to Cartesian coordinates. To convert
%                   N points, points is a 3XN matrix with each column
%                   having the format [latitude;longitude; height].
%                   Alternately, one can just provide points as
%                   [latitude;longitude], where height is implied as zero,
%                   and the return values are 2D geocentric (latitude
%                   longitude pairs). Also, one can just provide points as
%                   latitudes, in which case spherical latitudes for points
%                   on the surface of the reference ellipsoid are returned.
%           a       The semi-major axis of the reference ellipsoid. If
%                   this argument is omitted, the value in
%                   Constants.WGS84SemiMajorAxis is used.
%           f       The flattening factor of the reference ellipsoid. If
%                   this argument is omitted, the value in
%                   Constants.WGS84Flattening is used.
%
%OUTPUTS:   points  A matrix of the converted points. Each column of the
%                   matrix has the format [r;azimuth;elevation],
%                   with azimuth and elevation given in radians, unless the
%                   input points were given as 2D [latitude;longitude]
%                   points, in which case the output points are of the form
%                   [azimuth;elevation], unless the input points were just
%                   given as latitudes, in which case the output points are
%                   just elevations (geocentric latitudes).

f = 1/298.257223563;    %flattening
a = 6378136.3;          %seminmajor [m]

pDim=size(points,1);
if(pDim<3)%If only latitude and longitudes or just latitudes are provided.
    e2=2*f-f^2;%The square of the eccentricity of the reference ellipse.
    geodetLat=points(1,:);
    
    geocenLat=asin(sign(geodetLat).*sqrt((e2-1)^2*sin(geodetLat).^2./(1+e2*(e2-2)*sin(geodetLat).^2)));
    
    if(pDim==2)
        points(1,:)=points(2,:);
        points(2,:)=geocenLat;
    else
        points=geocenLat;
    end
else

    cartPoints = ellips2Cart(points,a,f);
    x=cartPoints(1,:);
    y=cartPoints(2,:);
    z=cartPoints(3,:);
    r=sqrt(x.^2+y.^2+z.^2);
    azimuth=atan2(y,x);
    elevation=asin(z./r);
    points=[r;azimuth;elevation];

end
end
