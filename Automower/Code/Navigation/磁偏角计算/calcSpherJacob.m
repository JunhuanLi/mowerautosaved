function J=calcSpherJacob(point)
%#codgen
%%CALCSPHERJACOB  Compute the Jacobian matrix for a point in spherical
%                 [range;azimuth;elevation] coordinates.
%
%INPUTS: point   A point in the format [range;azimuth;elevation], where the
%                two angles are given in radians.
%
%OUTPUTS: J     The 3X3 Jacobian matrix. Each row is a components of range,
%               azimuth and elevation (in that order by row) with
%               derivatives taken with respect to [x,y,z] by column.

CartPoint=spher2Cart(point);
x=CartPoint(1);
y=CartPoint(2);
z=CartPoint(3);

r=point(1);
J=zeros(3,3);

%Derivatives with respect to x.
J(1,1)=x/r;
J(2,1)=-y/(x^2+y^2);
J(3,1)=-x*z/(r^2*sqrt(x^2+y^2));

%Derivatives with respect to y.
J(1,2)=y/r;
J(2,2)=x/(x^2+y^2);
J(3,2)=-y*z/(r^2*sqrt(x^2+y^2));

%Derivatives with respect to z.
J(1,3)=z/r;
J(2,3)=0;
J(3,3)=sqrt(x^2+y^2)/r^2;

end
