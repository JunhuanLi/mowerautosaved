function geoEastOfNorth=magHeading2Geog(points,PEastOfNorth)
%MAGHEADING2GEOG Convert a heading in terms of radians clockwise
%                (East) of MAGNETIC North to a heading in terms of radians
%                clockwise from GEOGRAPHIC North as defined on a particular
%                reference ellipsoid. The WMM2010 model for the Earth's
%                magnetic field is used. In the rare spots (i.e. near the
%                magnetic poles), where the magnetic field vector points
%                directly towards the reference ellipsoid, the geographic
%                heading will be undefined and a NaN will be returned.
%
%INPUTS: points  One or more points given in geodetic latitude and
%                longitude, in radians, and height, in meters where the
%                magnetic headings apply. To convert N headings, points is
%                a 3XN matrix with each column having the format
%                [latitude;longitude; height]. All points should be
%                associated with the same time. At the geographic poles,
%                the longitude value determines the orientation of the
%                local coordinate system axes. Thus, geographic headings
%                ARE defined at the poles.
%  PEastOfNorth  An NX1 array of N magnetic headings in radians clockwise
%                from North that should be turned into geographic headings.
%    year        A decimal number indicating a year in the Gregorian
%                calendar as specified by UTC. For example, halfway through
%                the year 2012 would be represented as 2012.5. The
%                precision of the model is not sufficiently fine that leap
%                seconds matter. If this parameter is omitted, then the
%                last reference epoch of the geomagnetic model is used.
%           a    The semi-major axis of the reference ellipsoid. If
%                this argument is omitted, the value in
%                Constants.WGS84SemiMajorAxis is used.
%           f    The flattening factor of the reference ellipsoid. If
%                this argument is omitted, the value in
%                Constants.WGS84Flattening is used.
%
%OUTPUT: geoEastOfNorth The headings converted to radians clockwise of
%                       geographic North on the reference ellipse.

f = 1/298.257223563;    %flattening
a = 6378136.3;          %seminmajor [m]
aH = 6371200;
cH = 40592189440000.0;
C = [0,0,0,0,0,0,0,0,0,0,0,0,0;-1.70298699501787e-05,-9.15850732015503e-07,0,0,0,0,0,0,0,0,0,0,0;-1.07179210297520e-06,1.35331306134242e-06,7.46220605451230e-07,0,0,0,0,0,0,0,0,0,0;5.06510190279666e-07,-8.79220957114064e-07,4.65614434300067e-07,2.39629475887850e-07,0,0,0,0,0,0,0,0,0;3.04200000000000e-07,2.69633333333333e-07,5.55666666666667e-08,-1.19033333333333e-07,2.98000000000000e-08,0,0,0,0,0,0,0,0;-6.96189694630056e-08,1.07699852283177e-07,6.03927223189261e-08,-4.25432507199225e-08,-4.91463491661755e-08,-2.35178848770656e-09,0,0,0,0,0,0,0;2.01910871425983e-08,1.90262167305254e-08,2.10786074565587e-08,-3.92173038731237e-08,-6.32358223696761e-09,3.66102129508651e-09,-2.16055726429727e-08,0,0,0,0,0,0;2.07850106246465e-08,-1.93907366200118e-08,-1.21353478181166e-09,1.16964097055464e-08,3.58896456748554e-09,2.68526845337048e-09,4.38938112570174e-10,1.26517455976109e-09,0,0,0,0,0;5.91786925088652e-09,1.96453856279430e-09,-3.51676656302683e-09,-1.35819950020346e-09,-4.68093756320123e-09,2.78915968791783e-09,2.64363831289603e-09,-3.41975231301230e-09,-8.97381812634432e-10,0,0,0,0;1.23884496290103e-09,2.15650789838328e-09,7.80013495159910e-10,-1.19296181612692e-09,7.11188774998742e-10,-2.84475509999497e-09,-1.60591013709393e-10,1.92709216451272e-09,-1.95003373789978e-09,-2.31709891209267e-09,0,0,0;-4.36435780471985e-10,-1.37477270848675e-09,1.96396101212393e-10,-2.40039679259592e-10,-4.36435780471985e-11,5.45544725589981e-10,-6.54653670707977e-11,4.80079358519183e-10,6.76475459731577e-10,-2.18217890235992e-10,-6.11010092660779e-10,0,0;6.25543242171224e-10,-3.12771621085612e-10,-4.37880269519857e-10,3.54474503897027e-10,-1.04257207028537e-10,1.04257207028537e-10,-1.66811531245660e-10,8.34057656228299e-11,3.75325945302735e-10,2.08514414057075e-11,1.45960089839952e-10,7.92354773416884e-10,0;-4.40000000000000e-10,-4.00000000000000e-11,6.00000000000000e-11,2.00000000000000e-10,-1.20000000000000e-10,1.80000000000000e-10,-2.00000000000000e-11,1.00000000000000e-10,-8.00000000000000e-11,-8.00000000000000e-11,4.00000000000000e-11,-1.60000000000000e-10,0];
S = [0,0,0,0,0,0,0,0,0,0,0,0,0;0,2.85465067098119e-06,0,0,0,0,0,0,0,0,0,0,0;0,-1.21092025253524e-06,-2.57639752367526e-07,0,0,0,0,0,0,0,0,0,0;0,-6.05499085760782e-08,9.52092507510243e-08,-2.02815736216751e-07,0,0,0,0,0,0,0,0,0;0,9.54666666666667e-08,-7.04000000000000e-08,5.47666666666667e-08,-1.03033333333333e-07,0,0,0,0,0,0,0,0;0,1.34474059681683e-08,5.69554929907396e-08,-3.56386409290917e-08,0,3.04224946678964e-08,0,0,0,0,0,0,0;0,-5.76888204074238e-09,1.22311393267663e-08,1.70570310339258e-08,-1.83883115048663e-08,8.59785304149105e-10,1.52542553961938e-08,0,0,0,0,0,0;0,-1.49497157163606e-08,-5.44799657366510e-09,1.67829278335655e-09,6.42915235470431e-09,1.80739222823013e-09,-7.15210924599636e-09,-8.52056336165632e-10,0,0,0,0,0;0,2.66789187539966e-09,-4.85071250072666e-09,2.88617393793236e-09,-4.22011987563219e-09,4.05034493810676e-09,1.69774937525433e-09,-2.61938475039240e-09,4.12310562561766e-10,0,0,0,0;0,-4.70302254434652e-09,2.63828093951146e-09,2.93652139354319e-09,-1.65179328386804e-09,-1.69767643064216e-09,1.83532587096449e-09,4.81773041128180e-10,-1.39943597661043e-09,1.60591013709393e-09,0,0,0;0,6.11010092660779e-10,-2.18217890235992e-11,1.02562408410916e-09,9.60158717038367e-10,-1.57116880969915e-09,-2.18217890235992e-10,-8.51049771920370e-10,-4.36435780471985e-10,-4.36435780471985e-10,-1.81120848895874e-09,0,0;0,4.17028828114150e-11,3.54474503897027e-10,-1.25108648434245e-10,-3.75325945302735e-10,1.87662972651367e-10,-8.34057656228299e-11,-5.21286035142687e-10,-2.71068738274197e-10,-4.37880269519857e-10,-3.96177386708442e-10,-3.75325945302735e-10,0;0,-1.80000000000000e-10,6.00000000000000e-11,4.20000000000000e-10,-5.00000000000000e-10,1.00000000000000e-10,1.20000000000000e-10,0,2.00000000000000e-11,6.00000000000000e-11,-1.80000000000000e-10,-4.00000000000000e-11,1.80000000000000e-10];

[~,gradV]=spherHarmonicEval(C,S,ellips2Sphere(points),aH,cH);
B=-gradV;
%B is now a matrix of vectors of the magnetic flux of the Earth's field at
%the points. The direction of B in the local tangent plane to the reference
%ellipsoid approximately defined magnetic north.

numPoints=size(points,2);
geoEastOfNorth=zeros(numPoints,1);%Allocate space
for curPoint=1:numPoints
    %-B(:,curPoint) defines the direction of magnetic North at the current
    %point. We want the direction vector PEastOfNorth(curPoint). To go East
    %of North, one must rotate that number of degrees about the local
    %"Down" vector.
    u=getENUAxes(points(:,curPoint),a,f);
    
%     %The negative of u(:,3) is a unit vector in the down direction. 
    vRot = B(:,curPoint)*cos(PEastOfNorth(curPoint)) + cross(-u(:,3),B(:,curPoint))*sin(PEastOfNorth(curPoint))+-u(:,3)*dot(-u(:,3),B(:,curPoint))*(1-cos(PEastOfNorth(curPoint)));

    %Given the rotated field vector, only the components in the local
    %tangent plane matter.
    vEast=dot(vRot,u(:,1));
    vNorth=dot(vRot,u(:,2));
    %Find the angle East of North.
    geoEastOfNorth(curPoint)=atan2(vEast,vNorth);
end

end
