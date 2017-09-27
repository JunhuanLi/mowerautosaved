function q = eul2quatf(eul)
%#codegen

%Input:euler angle 3*1
%Output:quaternion 4*1

% Compute sines and cosines of half angles
c = cos(eul/2);
s = sin(eul/2);

% Construct quaternion
q = [c(1).*c(2).*c(3)+s(1).*s(2).*s(3);...
     c(1).*c(2).*s(3)-s(1).*s(2).*c(3);...
     c(1).*s(2).*c(3)+s(1).*c(2).*s(3);...
     s(1).*c(2).*c(3)-c(1).*s(2).*s(3)];
        
end

