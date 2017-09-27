function [P_updated,stateVec_updated] = tu_att_ekf(P,stateVec,angular_imu_rad_s,dt,Q)
%#codegen

%1st-order approximation
phi = [0 -angular_imu_rad_s(1)*dt -angular_imu_rad_s(2)*dt    -angular_imu_rad_s(3)*dt;...
       angular_imu_rad_s(1)*dt 0 angular_imu_rad_s(3)*dt      -angular_imu_rad_s(2)*dt;...
       angular_imu_rad_s(2)*dt   -angular_imu_rad_s(3)*dt  0   angular_imu_rad_s(1)*dt;...
       angular_imu_rad_s(3)*dt   angular_imu_rad_s(2)*dt      -angular_imu_rad_s(1)*dt 0];
%discrete transition matrix
F_discrete = (0.5 * phi + eye(length(stateVec)));
%noise transition matrix
G = 0.5*dt*[-stateVec(2) -stateVec(3) -stateVec(4);...
             stateVec(1) -stateVec(4) stateVec(3);...
            -stateVec(4) stateVec(1) -stateVec(2);...
            -stateVec(3) stateVec(2) stateVec(1)];
%discrete system noise variance   
Q_discrete = G * Q * G';
%time update(attitude propagation)
stateVec_updated = F_discrete * stateVec;
%P matrix calculation
P = F_discrete * P *(F_discrete') + Q_discrete;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
%guarantee P ,prevent filter diverse due to illness of P matrix
P_updated = 0.5*(P + transpose(P));
%guarantee main diagonal elements positive
for i=1:length(P_updated)
    if P_updated(i,i) < 0
        P_updated(i,i) = 0;
    end
end
end
