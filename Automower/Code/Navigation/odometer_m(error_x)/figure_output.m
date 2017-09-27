figure;plot(poseVec(1,:),poseVec(2,:));grid on;title('2D motion');xlabel('x north [m]');ylabel('y east [m]');
hold on;plot(ground_truth_x,ground_truth_y);legend('simulation result','ground truth');

figure;
subplot(311);plot(stateVec(1,:));title('stateVec1');grid on
subplot(312);plot(stateVec(2,:));title('stateVec2');grid on
subplot(313);plot(stateVec(3,:));title('stateVec3');grid on

figure;plot(norm_P(2:end));title('norm of P');grid on;
figure;plot(poseVec(3,:)*57.3);title('Theta angle [deg]');grid on

figure;
subplot(211);plot(poseVec(4,:));grid on;title('vx');hold on;plot(vel_truth_x);legend('simulation vel','true vel')
subplot(212);plot(poseVec(5,:));grid on;title('vy');hold on;plot(vel_truth_y);