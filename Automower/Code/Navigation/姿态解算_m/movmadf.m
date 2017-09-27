function [xmad,xmedian] = movmadf(x,k)
%#codegen
x = x(:);

%data head tructat
len = length(x);
xmad = zeros(len,1);
xmedian = zeros(len,1);

m = 1;
while(1)
    if (1<=m)&&(m<=k)
        x_window = x(1:m+k);
    elseif ((len-m)>=0)&&((len-m)<k)
        x_window = x(m-k:end);
    else 
        x_window = x(m-k:m+k);
    end
    %ignore samples that contain NaN and truncate at the borders of the input
    x_window(isnan(x_window)) = [];
    xmedian(m) = medianf(x_window);
    xmad(m) = medianf(abs(x_window-xmedian(m)));
    if (m == len)
        break;
    end
    m = m+1;
end

end