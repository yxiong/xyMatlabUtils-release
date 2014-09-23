function varargout = cart2sphere(varargin)

% USAGE:
%   sphere_coord = cart2sphere(cartisian_coord)
%   sphere_coord = cart2sphere(x,y,z)
%   [rho,theta,phi] = cart2sphere(cartisian_coord)
%   [rho,theta,phi] = cart2sphere(x,y,z)
%
% DESCRIPTION:
%   Compute the sphere coordinates from cartisian ones
%     rho = sqrt(x^2+y^2+z^2)
%     theta = arccos(z/rho)
%     phi = arctan(y/x)
%   Correspondingly,
%     x = rho * cos(phi) * sin(theta)
%     y = rho * sin(phi) * sin(theta)
%     z = rho * cos(theta)
%   Constraint on theta and phi:
%     0   <  theta <= pi
%     -pi <=  phi  <= pi
%
% INPUT:
%   cartisian_coord:  a 3XN matrix of cartisian coordinates
%   x,y,z:            1XN vector for cooresponding coordinates
%
% OUTPUT:
%   sphere_coord:     a 3XN matrix of sphere coordinates
%   rho,theta,phi:    1XN vector for cooresponding coordinates
%
%   Author: Ying Xiong
%   Created: Feb 04, 2012

if (nargin == 1)
  x = varargin{1}(1,:)';
  y = varargin{1}(2,:)';
  z = varargin{1}(3,:)';
elseif (nargin == 3)
  x = varargin{1};
  y = varargin{2};
  z = varargin{3};
else
  error('wrong input');
end

rho = sqrt(x.^2 + y.^2 + z.^2);
theta = acos(z ./ rho);
phi = atan2(y, x);

if (nargout == 1)
  varargout{1} = [rho'; theta'; phi'];
elseif (nargout == 3)
  varargout{1} = rho;
  varargout{2} = theta;
  varargout{3} = phi;
else
  error('wrong output');
end
