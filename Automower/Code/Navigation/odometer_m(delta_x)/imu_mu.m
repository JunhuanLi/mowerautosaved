function [stateVec_updated,poseVec_updated,P_updated,var_z]=imu_mu(poseVec_prev,stateVec,axb,ayb,w,P,R,dt)
%%
    theta = poseVec_prev(3);
    A = [1 0 0 dt 0 0;0 1 0 0 dt 0;0 0 1 0 0 0;0 0 0 1 0 0;0 0 0 0 1 0;0 0 0 0 0 0];
    ax = (axb*cos(theta)+ayb*sin(theta));
    ay = (axb*sin(theta)-ayb*cos(theta));
    B = [0.5*ax*dt^2;...
         0.5*ay*dt^2;...
         w*dt;...
         ax*dt;...
         ay*dt;...
         w];
    z = (A-eye(6))*poseVec_prev + B;
    var_z(1:6) = var(z)';
    H = eye(6);
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
    poseVec_updated = poseVec_prev + stateVec_updated;

end
