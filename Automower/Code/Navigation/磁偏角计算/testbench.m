clc;close all;
%heading to geo north in radian
tic
llh(1) = 22.67103865 * pi/180;
llh(2) = 113.90900656 * pi/180;
llh(3) = 0;
delta_heading_rad = magHeading2Geog(llh',0)
toc