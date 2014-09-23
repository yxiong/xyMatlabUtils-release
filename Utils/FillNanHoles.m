function D = FillNanHoles(varargin)

% D = FillNanHoles(D0, fillDirections, fillScales)
%
% Fill the NaN values in 'D' from its neighboring non-NaN values.
%
% INPUT:
%   D: a matrix whose entries may contain NaN.
%   fillDirections: a 2xN matrix specifying the filling directions, default
%                   [ 1 -1  0  0; ...
%                     0  0  1 -1].
%   fillScales: an Nx1 vector specifying the scale for each direction,
%               default ones(N, 1).
%
% OUTPUT:
%   D: a matrix of the same size with NaN values filled by neighbors.
%
% Example 1: Default filling with 4-neighbors with equal weights.
%   D = FillNanHoles(D);
%
% Example 2: Filling with 8-neighbors with varying weights.
%   d8 = [ 1 -1  0  0  1  1 -1 -1; ...
%          0  0  1 -1  1 -1 -1 1];
%   s8 = [ 2  2  2  2  1  1  1 1];
%   D = FillNanHolds(D, d8, s8);
%
%   Author: Ying Xiong.
%   Created: Nov 12, 2013.

% Warning only displayed once.
warning('FALLBACK:FillNanHoles', ...
        'Falling back to matlab .m file instead of the mex binary.\n');
warning('off', 'FALLBACK:FillNanHoles');

D = FillNanHoles_m(varargin{:});
