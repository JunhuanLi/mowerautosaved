function q = eul2quat1( eul, varargin )

% Pre-allocate output
q = zeros(size(eul,1), 4, 'like', eul);

% Compute sines and cosines of half angles
c = cos(eul/2);
s = sin(eul/2);

% The parsed sequence will be in all upper-case letters and validated
% switch seq
%     case 'ZYX'
        % Construct quaternion
        q = [c(:,1).*c(:,2).*c(:,3)+s(:,1).*s(:,2).*s(:,3), ...
            c(:,1).*c(:,2).*s(:,3)-s(:,1).*s(:,2).*c(:,3), ...
            c(:,1).*s(:,2).*c(:,3)+s(:,1).*c(:,2).*s(:,3), ...
            s(:,1).*c(:,2).*c(:,3)-c(:,1).*s(:,2).*s(:,3)];
        
%     case 'ZYZ'
%         % Construct quaternion
%         q = [c(:,1).*c(:,2).*c(:,3)-s(:,1).*c(:,2).*s(:,3), ...
%             c(:,1).*s(:,2).*s(:,3)-s(:,1).*s(:,2).*c(:,3), ...
%             c(:,1).*s(:,2).*c(:,3)+s(:,1).*s(:,2).*s(:,3), ...
%             s(:,1).*c(:,2).*c(:,3)+c(:,1).*c(:,2).*s(:,3)];
% end

end

