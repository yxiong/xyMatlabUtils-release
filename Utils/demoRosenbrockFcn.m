%   Author: Ying Xiong.
%   Created: Jan 20, 2014.

% Plot the contour near global minimum.
nSamples = 1001;
[xx, yy] = meshgrid(linspace(-5, 5, nSamples), linspace(-5, 5, nSamples));
ff = RosenbrockFcn(xx, yy);

figure; hold on; box on;
contour(xx, yy, log(ff));
plot(1, 1, '*');
title('Rosenbrock Function', 'FontSize', 14);
