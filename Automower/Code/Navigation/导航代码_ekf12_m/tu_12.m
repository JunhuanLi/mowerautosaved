function [P_updated,delta_stateVec_updated,stateVec_updated] = tu_12(P,delta_stateVec,stateVec,acc_measurement,dt,Q,quat)
%#codegen
%define matrix for short
I = eye(3);
Z = zeros(3);

%noise transition matrix
Fi = [Z Z;I Z;Z I;Z Z];

%rotation matrix
R = quat2dcm(quat');

% qw = quat(1);
% qx = quat(2);
% qy = quat(3);
% qz = quat(4);
% R = [qw^2+qx^2-qy^2-qz^2 2*(qx*qy-qw*qz) 2*(qx*qz+qw*qy);...
%       2*(qx*qy+qw*qz) qw^2-qx^2+qy^2-qz^2 2*(qz*qy-qw*qx);...
%       2*(qx*qz-qw*qy) 2*(qz*qy+qw*qx) qw^2-qx^2-qy^2+qz^2];


% system transition matrix
% A = [Z I Z Z Z Z;Z Z -R*cross3(acc_measurement-stateVec(11:13)) -R Z I;...
%      Z Z cross3(angular_rate_measurement-stateVec(14:16)) Z -I Z;...
%      Z Z Z Z Z Z;Z Z Z Z Z Z;Z Z Z Z Z Z];
%µÚÈýÐÐ£¿formula 172 and 195
 
%discretization by the simplest Euler form(1st order)
Fx = [I I*dt Z Z;...
      Z I -R*dt I*dt;...
      Z Z I Z;...
      Z Z Z I];

% %discrete transition matrix by 2nd order
% Fx_discrete = eye(length(Fx)) + Fx * dt + (Fx).^2*dt^2/2;  
% %discreete transition by approximate methods using truncated series
% %to be continued....

%% delta state propogation
delta_stateVec_updated = Fx*delta_stateVec;

%P matrix propagation
P = Fx * P * Fx' + Fi * Q * Fi';

%guarantee P ,prevent filter diverse due to illness of P matrix
P_updated = 0.5*(P + transpose(P));

%guarantee main diagonal elements positive
for i=1:length(P_updated)
    if P_updated(i,i) < 0
        P_updated(i,i) = 0;
    end
end

%true state propogation
stateVec_updated = zeros(12,1);
stateVec_updated(1:3) = stateVec(1:3) + stateVec(4:6)*dt + 0.5 * dt^2 * (R*(acc_measurement-stateVec(7:9)) + stateVec(10:12));
stateVec_updated(4:6) = stateVec(4:6) + (R*(acc_measurement-stateVec(7:9)) + stateVec(10:12))*dt;
stateVec_updated(7:end) = stateVec(7:end);

end