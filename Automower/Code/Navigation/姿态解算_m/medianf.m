function y = medianf(x)
%#codegen
x = x(:);
x(isnan(x)) = [];
xsort = sortf(x);
len = length(xsort);
if mod(len,2)==1
   y = xsort((len+1)/2) ;
else 
   y = mean([xsort(len/2),xsort(len/2+1)]);
end

end