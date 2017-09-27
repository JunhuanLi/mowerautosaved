%%butterworth
Wn = 5/10;  % normalized cutoff frequency
N = 3;                    % N-order filter
[b,aa] = butter(N,Wn,'low');
imu_mx_lp_chunhua_off_motro_off = filter(b,aa,imu_mx_chunhua_off_motro_off);
figure;
plot(imu_mx_chunhua_off_motro_off);hold on;plot(imu_mx_lp_chunhua_off_motro_off);grid on;title('mx');legend('mx','low-pass filterd mx');