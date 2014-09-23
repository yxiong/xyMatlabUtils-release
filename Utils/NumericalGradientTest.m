function NumericalGradientTest()

%   Author: Ying Xiong.
%   Created: May 10, 2014.

rng(0);
%% Compare between analytical and numerical gradient.
x0 = randn(5, 1);
% Forward difference.
options = struct('dx', 1e-4, 'method', 'forward');
[ng, nf0] = NumericalGradient(@(x)NumericalGradientCost(x), x0, options);
[af0, ag] = NumericalGradientCost(x0);
CheckNear(af0, nf0, 1e-12);
CheckNear(ng, ag, 1e-3);
% Central difference (higher accuracy).
options = struct('dx', 1e-4, 'method', 'central');
[ng, nf0] = NumericalGradient(@(x)NumericalGradientCost(x), x0, options);
[af0, ag] = NumericalGradientCost(x0);
CheckNear(af0, nf0, 1e-12);
CheckNear(ng, ag, 1e-6);

fprintf('Passed.\n');
end

function [f, g] = NumericalGradientCost(x)

y = sum(x.^2);
f = sin(y) + 2*cos(y) - 3*exp(-y.^2) + log(4*y);

if (nargout > 1)
  gy = cos(y) - 2*sin(y) + 3*y*exp(-y.^2) + 1/y;
  g = gy * 2*x;
end
  
end
