function [pos,vel,P,a_n]=ekf9_tu(acc,acc_bias,pos_prev,vel_prev,quat,dt,P,Q,tau_acc)
%% ������
axis_a = 6378137;             %���������뾶[m]

%% �������������������
g_n  = [0,0,9.8005]';

%% λ�����ٶȽ���
Cnb = quat2dcm(quat)';
acc_biased = acc + acc_bias;
a_n = Cnb*acc_biased + g_n  ;
% az = a_n(3);
% a_n = acc_biased + g_n  ;
vel = vel_prev + a_n*dt;   
  
%���ι�ʽ����λ��
pos(1) = pos_prev(1) + dt*0.5*(vel(1) + vel_prev(1));
pos(2) = pos_prev(2) + dt*0.5*(vel(2) + vel_prev(2));
pos(3) = pos_prev(3) + dt*0.5*(vel(3) + vel_prev(3));

%% Kalman filter time update
%denote
Z3 = zeros(3,3);    
I3 = eye(3);

%״̬ת�ƾ���
g_mag = norm(g_n);                          %   Local gravity magnitude    
% dp2dp = -skew(wn_en);                       %   Pos. to pos. error block
dp2dv = diag((g_mag/axis_a)*[-1 -1 2]);        %   Pos. to vel. error block 
% dv2dv = -skew(2*wn_ie + wn_en);             %   Vel. to vel. error block
da2dv = Cnb ;                               %   Accel. bias to vel. error block                        
da2da = -I3./tau_acc;                        %   Accl. bias to accel bias error block
% da2da = eye();                        %   Accl. bias to accel bias error block

% % % general formula
% phi = [   dp2dp      I3          Z3    ;...
%           dp2dv      dv2dv       da2dv ;...
%           Z3         Z3          da2da ];

% for small dt
phi = [   Z3      I3       Z3    ;...
          dp2dv   Z3       da2dv ;...
          Z3      Z3       da2da ];

%״̬ת�ƾ�����ɢ��
FDisc = eye(length(phi(1,:))) + phi*dt + ((phi*dt)^2)/2;
% FDisc = phi;

% G��ϵͳ������Ȩ����
G = [ Z3  Z3  ;...
      Cnb Z3  ;...
      Z3  I3  ];
% G = [ Cnb  Z3  ;...
%       Z3 Cnb  ;...
%       Z3  Z3  ];


%������ɢQ����
QDisc = 0.5 *dt*(FDisc * G * Q * G'* FDisc'+ G * Q * G');
% QDisc = G * Q * G';

%P�������
P = FDisc * P *(FDisc') + QDisc;
 
%��֤P����Գ���,��ֹ��Ϊ��̬���������˲�����ɢ
P = 0.5*(P + transpose(P));

%��֤�Խ���Ԫ��Ϊ��
for i=1:9
    if P(i,i) < 0
        P(i,i) = 0;
    end
end

end