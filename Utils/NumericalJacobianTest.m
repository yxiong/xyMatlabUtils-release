function NumericalJacobianTest()

%   Author: Ying Xiong.
%   Created: Feb 05, 2014.

rng(0);
%% Compare between analytical and numerical Jacobian matrix.
x0 = randn(5, 1);
% Forward difference.
options = struct('dx', 1e-4, 'method', 'forward');
[nJ, nf0] = NumericalJacobian(@(x)NumericalJacobianCost(x), x0, options);
[af0, aJ] = NumericalJacobianCost(x0);
CheckNear(af0, nf0, 1e-12);
CheckNear(nJ, aJ, 1e-3);
% Central difference (higher accuracy).
options = struct('dx', 1e-4, 'method', 'central');
[nJ, nf0] = NumericalJacobian(@(x)NumericalJacobianCost(x), x0, options);
[af0, aJ] = NumericalJacobianCost(x0);
CheckNear(af0, nf0, 1e-12);
CheckNear(nJ, aJ, 1e-6);

fprintf('Passed.\n');
end

function [f, J] = NumericalJacobianCost(x)

y = sum(x.^2);
f = [sin(y); cos(y); exp(-y.^2); log(y)];

if (nargout > 1)
  Jy = [cos(y); -sin(y); -2*y*exp(-y.^2); 1/y];
  J = Jy * 2*x';
end
  
end
