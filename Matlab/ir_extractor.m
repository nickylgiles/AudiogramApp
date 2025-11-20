clear; clc;

sofaFile = 'dtf_nh2.sofa';
outputFolder = 'HRIRs_test';
mkdir(outputFolder)

s = sofaread(sofaFile);
fs = s.SamplingRate;

zeroElevationIndices = find(s.SourcePosition(:,2) == 0);
fprintf('Number of indices is %d', length(zeroElevationIndices))

for i = 1:length(zeroElevationIndices)
    j = zeroElevationIndices(i);
    az = round(s.SourcePosition(j, 1));

    hL = squeeze(s.Numerator(j, 1, :));
    hR = squeeze(s.Numerator(j, 2, :));
    
    filenameL = fullfile(outputFolder, sprintf('HRIR_%d_L.wav', az));
    filenameR = fullfile(outputFolder, sprintf('HRIR_%d_R.wav', az));

    audiowrite(filenameL, hL, fs);
    audiowrite(filenameR, hR, fs);
end