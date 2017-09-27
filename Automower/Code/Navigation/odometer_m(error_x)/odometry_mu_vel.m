function [stateVec_updated,poseVec_updated,P_updated]=odometry_mu_vel(poseVec,stateVec,delta_odo_r,delta_odo_l,P,R,D,dt)

%    H = Hx*X_delta_x;   X_delta_x = I6;
%%
    theta = poseVec(3);
    odo = [delta_odo_r,delta_odo_l]';
    delta_x = cos(theta+(odo(1)-odo(2))/2/D)*0.5*(odo(1)+odo(2));
    delta_y = sin(theta+(odo(1)-odo(2))/2/D)*0.5*(odo(1)+odo(2));
    delta_theta = (odo(1)-odo(2))/D;
    z = [delta_x;delta_y;delta_theta];
    z = [z(1:2)/dt];
    H = [0 0 0 1 0 0;0 0 0 0 1 0];
    R = diag([0.01,0.01])*100;
    K = P * H'/((H * P * H' + R ));
    stateVec_updated = stateVec + K*(z - H*stateVec);
    P = (eye(size(stateVec,1)) - K * H)* P;
    P_updated = 0.5*(P + transpose(P));
    for j=1:length(P)
        if P_updated(j,j) < 0
            P_updated(j,j) = 0;
        end
    end
%     pose vector updated
    poseVec_updated = poseVec + stateVec_updated;


%     poseVec_updated(1:5) = poseVec(1:5) + z;
% poseVec_updated(6) = poseVec(6) ;
% stateVec_updated = poseVec_updated;
% P_updated = P;
end
