function eul = quat2eul1( q, varargin )
% Normalize the quaternions
% q = robotics.internal.normalizeRows(q);
q = q/norm(q);
qw = q(:,1);
qx = q(:,2);
qy = q(:,3);
qz = q(:,4);

% Pre-allocate output
eul = zeros(size(q,1), 3, 'like', q);
% seq = 1;
% The parsed sequence will be in all upper-case letters and validated
% switch seq
%     case 'ZYX'
        % Cap all inputs to asin to 1, since values >1 produce complex
        % results
        % Since the quaternion is of unit length, this should never happen,
        % but some code generation configuration seem to hit this edge case
        % under some circumstances.
        aSinInput = -2*(qx.*qz-qw.*qy);
        aSinInput(aSinInput > 1) = 1;
        
        eul = [ atan2( 2*(qx.*qy+qw.*qz), qw.^2 + qx.^2 - qy.^2 - qz.^2 ), ...
            asin( aSinInput ), ...
            atan2( 2*(qy.*qz+qw.*qx), qw.^2 - qx.^2 - qy.^2 + qz.^2 )];
        
%     case 'ZYZ'
%         % Need to convert to intermediate rotation matrix here to avoid
%         % singularities
%         R = quat2rotm(q);
%         eul = rotm2eul(R, 'ZYZ');
% end

% Check for complex numbers
if ~isreal(eul)
    eul = real(eul);
end

end
