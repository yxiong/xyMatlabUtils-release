function inBBox = BBoxContains(bbox, points)

% Check whether the points are contained in the bounding box.
% 
% USAGE:
%   inBBox = BBoxContains(bbox, points)
%
% INPUT:
%   bbox: 1x4 array describing the bounding box (see README).
%   points: Nx2 array, each row is a 2D point to be tested.
% OUTPUT:
%   inBBox: Nx1 array, each entry shows whether the corresponding point lies
%           inside the bounding box or not (true for in, flase for not).
%
%   Author: Ying Xiong
%   Created: Sep 11, 2012

inBBox = (bbox(1) <= points(:,1)) & ...
         (bbox(2) >  points(:,1)) & ...
         (bbox(3) <= points(:,2)) & ...
         (bbox(4) >  points(:,2));
