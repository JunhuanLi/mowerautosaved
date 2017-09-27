function [K_norm,P_updated,delta_stateVec_updated,stateVec_updated] = gps_mu_12(P,delta_stateVec,pos_gps_lla,vel_gps_ned,stateVec,R,pos_lla_ref)
%#codegen
%gps output convert to ned frame
gps_pos_ecef = wgslla2xyz(pos_gps_lla(1),pos_gps_lla(2),pos_gps_lla(3));
gps_pos_ned = wgsxyz2ned(gps_pos_ecef,pos_lla_ref(1),pos_lla_ref(2),pos_lla_ref(3));
measurement_gps = [gps_pos_ned;vel_gps_ned];

%method 1
% H = [eye(6) zeros(6,13)];
% K = P * H'/(H * P * H'+R);
% Z = delta_stateVec(1:6) - measurement_gps;
% stateVec_updated = K * (Z - H * delta_x);
% P = (eye(length(P)) - K * H) * P;

%method2
%relationship function between gps measurements and full states
h = [eye(6) zeros(6,6)];    

% %jocobian function calculation
% stateVec_delta_stateVec = eye(12);
% stateVec_delta_stateVec(1:6,1:6) = eye(6);
% stateVec_delta_stateVec(7:10,7:9) = q_delta_theta(stateVec(7:10));
% stateVec_delta_stateVec(11:end,10:end) = eye(9);
% 
% %relationship function between measurements and delta_states
% H = h*stateVec_delta_stateVec;    %derivative of h w.r.t. delta_x
H = h;   %due to stateVec_delta_stateVec = eye(12)

% %Kalman gain 
K = P * H'/(H * P * H' + R);

%delta states update
delta_stateVec_updated = K * (measurement_gps - h * stateVec);

% P matrix update
P = (eye(length(P)) - K * H) * P;
% P = (eye(length(P)) - K * H) * P * (eye(length(P)) - K * H)' + K * R * K';
% 
% %full state update
stateVec_updated = stateVec + delta_stateVec_updated;

% disp(quat2eul(stateVec(7:10)')*57.3)
% disp(quat2eul(stateVec_updated(7:10)')*57.3)
% disp(delta_stateVec_updated(7:9)'*57.3)
% disp(eul2quat(delta_stateVec_updated(7:9)'))

%% EKF reset
% G = eye(12);
% % G(7:9,7:9) = eye(3) - cross3(0.5*delta_stateVec(7:9));   %confirmed?
% 
% %delta states reset
% delta_stateVec_updated = zeros(18,1);
% %delta_x_updated(7:10) = zeros(4,1);    %only reset the orientation part??
% 
% %P matrix reset
% P = G * P * G';
%as G = I;

%guarantee P ,prevent filter diverse due to illness of P matrix
P_updated = 0.5*(P + transpose(P));
%guarantee main diagonal elements positive
for i=1:length(P_updated)
    if P_updated(i,i) < 0
        P_updated(i,i) = 0;
    end
end

K_norm = norm(K);


end

