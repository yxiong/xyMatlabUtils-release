%   Author: Ying Xiong.
%   Created: Aug 26, 2013.

%% Setup.
rng(0);
nTheta = 10;
r_gt = 20;
xc_gt = 10.3;
yc_gt = -15.2;
sigma = 0.1;   % Noise ratio.
TOL = 0.1;

%% Generate data.
theta = rand(nTheta, 1) * 2*pi;
x = cos(theta) * r_gt + xc_gt + randn(nTheta, 1) * sigma;
y = sin(theta) * r_gt + yc_gt + randn(nTheta, 1) * sigma;

%% Do the optimization.
[xc, yc, r] = FitCircle(x, y);

%% Check results.
CheckNear(xc, xc_gt, TOL);
CheckNear(yc, yc_gt, TOL);
CheckNear(r, r_gt, TOL);
fprintf('Passed.\n');
