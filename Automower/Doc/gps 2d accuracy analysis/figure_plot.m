% lat = Data(:,1)*0.01;
% lon = Data(:,2)*0.01;
% height = Data(:,3);

for k = 1:length(mean_pos)
    flatearth_pos(k,:) = lla2flat([mean_pos(k,1) mean_pos(k,2) mean_pos(k,3)], [mean_pos(1,1) mean_pos(1,2)], 0, mean_pos(1,3));
end
% ave_pos = [mean(flatearth_pos(:,1)),mean(flatearth_pos(:,2))];
figure;
scatter(flatearth_pos(:,2),flatearth_pos(:,1));
grid on;
xlabel('E direction [m]');ylabel('N direction [m]');
title('Each point = average position for 12h+ (Tallysman receiver)');

mean2 = [mean(data(:,1))*0.01 mean(data(:,2))*0.01 mean(data(:,3))];
