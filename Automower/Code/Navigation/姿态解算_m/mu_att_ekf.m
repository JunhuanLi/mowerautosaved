function [P_updated,stateVec_updated] = mu_att_ekf(P,acc_imu_g,stateVec,R)
%#codegen
%jocobian matrix
H = -2*[-stateVec(3) stateVec(4) -stateVec(1) stateVec(2);...
       stateVec(2) stateVec(1) stateVec(4) stateVec(3);...
       stateVec(1) -stateVec(2) -stateVec(3) stateVec(4)];
%kalman filter
K = P * H'/((H * P * H' + R ));
%measurement
measurement_acc = acc_imu_g/norm(acc_imu_g);
%state vector correction
stateVec_updated = stateVec + K * (measurement_acc - H * stateVec); 
%P matrix correction
P = (eye(length(P)) - K * H)* P;
%guarantee P ,prevent filter diverse due to illness of P matrix
P_updated = 0.5*(P + transpose(P));
%guarantee main diagonal elements positive
for i=1:length(P)
    if P_updated(i,i) < 0
        P_updated(i,i) = 0;
    end
end


end
