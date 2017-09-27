function [u,c]=getENUAxes(plhPoint,a,f)
%#codegen
%%GETENUAXES Find the East-North-Up unit vectors at the given point as well
%            as the magnitudes of the derivatives of a position vector with
%            respect to latitude, longitude and height.
%
%INPUTS:   plhPoint The point at which the axes are to be found given in
%                   terms of [latitude;longitude;height] with the geodetic 
%                   latitude and longitude in radians and the height in
%                   meters. The latitude should be between -pi/2 and pi/2.
%      justVertical An optional parameter. If this is given and is
%                   true, then u and c only for the Up direction will be
%                   returned.
%           a       The semi-major axis of the reference ellipsoid. If
%                   this argument is omitted, the value in
%                   Constants.WGS84SemiMajorAxis is used.
%           f       The flattening factor of the reference ellipsoid. If
%                   this argument is omitted, the value in
%                   Constants.WGS84Flattening is used.
%
%OUTPUTS:   u       u(:,1), u(:,2) and u(:,3) are respectively the East,
%                   North and Up unit vectors.
%           c       c(1), c(2) and c(3) are the respective magnitudes of
%                   the derivative of the Cartesian position with respect
%                   to latitude, longitude and height.

%The latitude
phi=plhPoint(1);
%The longitude
lambda=plhPoint(2);
%The height
h=plhPoint(3);

sinP=sin(phi);
cosP=cos(phi);
sinL=sin(lambda);
cosL=cos(lambda);

%u3 is dr/dh (Up)
u3=[cosP*cosL;
    cosP*sinL;
    sinP];
c3=norm(u3);%Barring precision problems, this is always one.
u3=u3/c3;

%The square of the first numerical eccentricity
e2=2*f-f^2;
%The normal radius of curvature.
Ne=a/sqrt(1-e2*sinP^2);
%The derivative of the normal radius of curvature with respect to phi.
dNedPhi=a*e2*cosP*sinP/(1-e2*sinP^2)^(3/2);

%u1 is dr/dlambda, normalized (East).
u1=[-(Ne+h)*cosP*sinL;
     (Ne+h)*cosP*cosL;
    0];
c1=norm(u1);

%u2 is dr/dphi, normalized (North)
u2=[(cosP*dNedPhi-(Ne+h)*sinP)*cosL;
    (cosP*dNedPhi-(Ne+h)*sinP)*sinL;
    (Ne*(1-e2)+h)*cosP+(1-e2)*dNedPhi*sinP];
c2=norm(u2);
u2=u2/c2;
    
%If the point is too close to the poles, then it is possible that c1 is
%nearly equal to zero. However, u1 can just be found by orthogonality:
%it is orthogonal to u3 and u2.
u1=cross(u2,u3);
u=[u1,u2,u3];
c=[c1;c2;c3];

end