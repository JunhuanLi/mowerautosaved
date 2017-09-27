function x = sortf(x)
%#codegen
len = length(x);
for m = 1:len-1
    for n = 1:len-m
        if x(n+1)<x(n)
            temp = x(n);
            x(n) = x(n+1);
            x(n+1) = temp;
        end
    end
end
end