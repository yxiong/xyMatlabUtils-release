function PlotBBox(varargin)

% Plot a bounding box.
%
% USAGE:
%   PlotBBox(bbox)
%   PlotBBox(bbox, spec)
%   PlotBBox(ax, bbox, spec)
% INPUT:
%   ax: the axis handle to be drawn on, default {gca}.
%   bbox: Nx4 array, each line represents a bounding box (see README).
%   spec: the specification used by plot function, default {'b'}.
%
%   Author: Ying Xiong
%   Created: Sep 11, 2012

% Parse input.
if (isscalar(varargin{1}))
  assert(ishandle(varargin{1}));
  ax = varargin{1};
  k = 2;
else
  ax = gca;
  k = 1;
end
bbox = varargin{k};
k = k + 1;
if (nargin >= k)
  spec = varargin{k};
else
  spec = 'b';
end

assert(size(bbox,2) == 4);

% Do the plot.
for i = 1:size(bbox, 1)
  x = [bbox(i, 1) bbox(i, 1) bbox(i, 2) bbox(i, 2) bbox(i, 1)];
  y = [bbox(i, 3) bbox(i, 4) bbox(i, 4) bbox(i, 3) bbox(i, 3)];
  plot(ax, x, y, spec); hold on;
end
