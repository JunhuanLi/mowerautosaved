function [stateVec_updated,poseVec_updated,P_updated,z]=odometry_mu(poseVec_prev,poseVec,stateVec,delta_odo_r,delta_odo_l,P,R,D,dt)
%    H = Hx*X_delta_x;   X_delta_x = I6;
%%
    theta = poseVec(3);
    r = delta_odo_r;
    l = delta_odo_l;
    if r~=l
        alpha = (l-r)/D;
        delta_x = (D+2*l/alpha)*cos(theta+alpha/2)*sin(alpha/2);
        delta_y = (D+2*l/alpha)*sin(theta+alpha/2)*sin(alpha/2);
        delta_theta = alpha;
    else
        alpha = 0;
        delta_x = cos(theta)*r;
        delta_y = sin(theta)*r;
        delta_theta = alpha;      
    end
    
    z1 = [delta_x;delta_y;delta_theta];
    delta_x = cos(theta+(r-l)/2/D)*0.5*(r+l);
    delta_y = sin(theta+(r-l)/2/D)*0.5*(r+l);
    delta_theta = (l-r)/D;
    z = [delta_x;delta_y;delta_theta]-(poseVec(1:3)-poseVec_prev(1:3));
%     z = [z;z(1:2)/dt];
%     H = [eye(5) zeros(5,1)];
%     R = diag([4e-3,0.0023,2e-3,0.1,0.1])*.01;

%     H = [1 0 -sin(theta+(odo(1)-odo(2))/2/D)*0.5*(odo(1)+odo(2)) 0 0 0;...
%          0 1 -cos(theta+(odo(1)-odo(2))/2/D)*0.5*(odo(1)+odo(2)) 0 0 0;...
%          0 0 1 0 0 0];
    H = [eye(3) zeros(3)];
    K = P * H'/((H * P * H' + R ));
%     stateVec_updated = stateVec + K*(z - H*stateVec);
    stateVec_updated = K*z;
    P = (eye(size(stateVec,1)) - K * H)* P;
    P_updated = 0.5*(P + transpose(P));
    for j=1:length(P)
        if P_updated(j,j) < 0
            P_updated(j,j) = 0;
        end
    end
    
%     pose vector updated
    poseVec_updated = poseVec - stateVec_updated;

end
