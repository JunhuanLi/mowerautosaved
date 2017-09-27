% function [xfilt, xi, xmedian, xsigma] = hampel_my(x)
function xfilt = hampelf(x)
%#codegen
k = 3;
nsigma = 3;
x = x(:);

%filter size will be 2*k+1
[xmad,xmedian] = movmadf(x,k);

% % scale the MAD by ~1.4826 as an estimate of its standard deviation
scale = 1.482602218505602;
xsigma = scale*xmad;

% identify points that are either NaN or beyond the desired threshold
xi = ~(abs(x-xmedian) <= nsigma*xsigma);

% replace identified points with the corresponding median value
xf = x;
xf(xi) = xmedian(xi);

xfilt = xf;

end

