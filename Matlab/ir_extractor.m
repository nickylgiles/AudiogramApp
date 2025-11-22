clear; clc;

sofaFile = 'dtf_nh2.sofa';
outputFolder = 'HRIRs_test';
mkdir(outputFolder)

s = sofaread(sofaFile);
fs = s.SamplingRate;

sel = find(s.SourcePosition(:,2) == 0);
fprintf('Number of indices is %d', length(sel))

for i = 1:length(sel)
    j = sel(i);
    az = round(s.SourcePosition(j, 1));

    hL = squeeze(s.Numerator(j, 1, :));
    hR = squeeze(s.Numerator(j, 2, :));
    
    filenameL = fullfile(outputFolder, sprintf('HRIR_%d_L.wav', az));
    filenameR = fullfile(outputFolder, sprintf('HRIR_%d_R.wav', az));

    audiowrite(filenameL, hL, fs);
    audiowrite(filenameR, hR, fs);
end


% Visualize IRs 
figure; hold on;

src = s.SourcePosition;

az  = deg2rad(src(:,1));
el  = deg2rad(src(:,2));
r   = src(:,3);

[x, y, z] = sph2cart(az, el, r);

scatter3(x, y, z, 10, 'b', 'filled');
hold on;

scatter3(x(sel), y(sel), z(sel), 20, 'r','filled','MarkerEdgeColor', 'k', 'LineWidth', 1.2);

rec = s.ReceiverPosition;

azR = deg2rad(rec(:,1));
elR = deg2rad(rec(:,2));
rR  = rec(:,3);

[xr, yr, zr] = sph2cart(azR, elR, rR);

scatter3(xr, yr, zr, 100, 'g', 'filled', 'MarkerEdgeColor','k');


xlabel('X'); ylabel('Y'); zlabel('Z');
title('Source Positions');
legend('All IRs','Selected IRs','Receiver');
grid on; axis equal;

rotate3d on

while true
    camorbit(0.5, 0);   % (azimuth, elevation)
    drawnow;
end