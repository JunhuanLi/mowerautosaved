function eul = quat2eulf(q)
%#codegen

%Input:quaternion 4*1
%Output:euler angle 3*1

% Normalize the quaternions
q = q/sqrt(q(1)*q(1)+q(2)*q(2)+q(3)*q(3)+q(4)*q(4));
qw = q(1);
qx = q(2);
qy = q(3);
qz = q(4);

aSinInput = -2*(qx.*qz-qw.*qy);
aSinInput(aSinInput > 1) = 1;
        
eul = [ atan2( 2*(qx.*qy+qw.*qz), qw.^2 + qx.^2 - qy.^2 - qz.^2 ); ...
        asin( aSinInput );...
        atan2( 2*(qy.*qz+qw.*qx), qw.^2 - qx.^2 - qy.^2 + qz.^2 )];

% Check for complex numbers
if ~isreal(eul)
    eul = real(eul);
end

end
