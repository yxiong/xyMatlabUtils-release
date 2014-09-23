function ufcn = BoundedFcnToUnconstrainedFcn(bfcn, lb, ub)

% ufcn = BoundedFcnToUnconstrainedFcn(bfcn, lb, ub)
%
% Change the function 'bfcn' with bounded input [lb ub] to an unconstrained
% function 'ufcn', such that
%       ufcn(y) = bfcn(mapfcn(y)),
% where 'mapfcn' is created by 'MapUnconstrainedToBounded'.
%
%   Author: Ying Xiong.
%   Created: Jan 29, 2014.

idx = find(isfinite(lb) | isfinite(ub));
mapfcn = MapUnconstrainedToBounded(lb, ub);
ufcn = @(y)BoundedFcnToUnconstrainedFcnSub(y, bfcn, lb, ub, mapfcn, idx);

end

function [f, g] = BoundedFcnToUnconstrainedFcnSub(y, bfcn, lb, ub, mapfcn, idx)

if (nargout == 1)
  x = mapfcn(y);
  f = bfcn(x);
else
  [x, dx] = mapfcn(y);
  [f, g] = bfcn(x);
  g(idx) = g(idx) .* dx(idx);
end

end
