%   Author: Ying Xiong.
%   Created: Nov 13, 2013.

rng(0);

%% Make sure the .cc and .m implementation works exactly the same way.
M = 30;
N = 50;
D0 = randn(M, N);
nTest = 5;
d8 = [ 1 -1  0  0  1  1 -1 -1; ...
       0  0  1 -1  1 -1 -1 1];
s8 = [ 2  2  2  2  1  1  1 1];
for iTest = 1:nTest
  D = D0;
  nanId = randi(numel(D0), [120 1]);
  D(nanId) = NaN;
  DDc = FillNanHoles(D);
  DDm = FillNanHoles_m(D);
  CheckNear(DDc, DDm, 1e-10);
  DDc = FillNanHoles(D, d8, s8);
  DDm = FillNanHoles_m(D, d8, s8);
  CheckNear(DDc, DDm, 1e-10);
end

%% Create a small number of holes and make sure we can fill them reasonably.
% Original image.
D0 = peaks(128);

% Create NaN holes.
TOL = 1e-2;
nanId = randi(numel(D0), [100, 1]);

% Do the check.
D = D0;
D(nanId) = NaN;
DD = FillNanHoles(D);
CheckNear(D0(nanId), DD(nanId), TOL);

% Create bigger holes.
TOL = 0.2;
nanId = randi(numel(D0), [10 1]);
nanImg = false(size(D0));
nanImg(nanId) = true;
se = strel('disk', 3);
nanImg = imdilate(nanImg, se);
nanId = find(nanImg);

% Do the check.
D = D0;
D(nanId) = NaN;
d8 = [ 1 -1  0  0  1  1 -1 -1; ...
       0  0  1 -1  1 -1 -1 1];
s8 = [ 2  2  2  2  1  1  1 1];
DD = FillNanHoles(D, d8, s8);
CheckNear(D0(nanId), DD(nanId), TOL);

fprintf('Passed.\n');
