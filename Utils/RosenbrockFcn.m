function varargout = RosenbrockFcn(varargin)

% [f, df_dx, df_dy] = RosenbrockFcn(x, y)
% [f, df] = RosenbrockFcn(xy)
%
% The Rosenbrock function is defined as
%   f(x,y) = (1-x)^2 + 100 (y - x^2)^2.
%
% Case 1: [f, df_dx, df_dy] = RosenbrockFcn(x, y)
%   x, y: two matrices/vectors/scalars of the same size.
%   f:    the function values, a matrix/vector/scalar of the same size as input.
%   df_dx, df_dy: the gradient of the function, of the same size as input.
%
% Case 2: [f, df] = RosenbrockFcn(xy)
%   xy: a 2xN matrix, with each column a data point.
%   f:  a Nx1 vector for the function values.
%   df: a 2xN matrix [df_dx'; df_dy'], for the gradient of the function at each
%       data point.
%
%   Author: Ying Xiong.
%   Created: Jan 20, 2014.

if (nargin == 1)
  xy = varargin{1};
  assert(size(xy, 1)==2);
  x = xy(1, :)';
  y = xy(2, :)';
elseif (nargin == 2)
  x = varargin{1};
  y = varargin{2};
  assert(ismatrix(x) && all(size(x)==size(y)));
else
  error('Wrong input!');
end

varargout{1} = (1-x).^2 + 100 * (y - x.^2).^2;

if (nargout > 1)
  df_dx = 2*(x-1) + 400 * x .* (x.^2 - y);
  df_dy = 200 * (y - x.^2);
  if (nargin == 1)
    varargout{2} = [df_dx'; df_dy'];
  elseif (nargin == 2)
    varargout{2} = df_dx;
    varargout{3} = df_dy;
  else
    error('Wrong input!');
  end
end
