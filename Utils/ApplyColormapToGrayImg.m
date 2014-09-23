function Iclr = ApplyColormapToGrayImg(Igray, clrmap, options)

% Iclr = ApplyColormapToGrayImg(Igray, clrmap, options)
%
% Apply a colormap 'clrmap' to a grayscale image 'Igray', turning it into a
% color image 'Iclr'.
%
% INPUT:
%   Igray: input grayscale image (MxN) with values ranging in [0, 1].
%   clrmap: an Kx3 colormap, e.g. jet(256).
%   options: a struct with following supported fields:
%     'NanClr': a 3-vector for NaN color, default clrmap(1,:).
%
% OUTPUT:
%   Iclr: a color image of size MxNx3 with values ranging in [0, 1].
%
% Example:
%   J = ApplyColormapToGrayImg(I, jet(256), struct('NanClr', [.5 .5 .5]));
%
%   Author: Ying Xiong.
%   Created: Mar 09, 2014.

% Setup default options.
if (~exist('options', 'var'))   options = struct();   end
if (~isfield(options, 'NanClr'))   options.NanClr = clrmap(1,:);   end

% Normalize input grayscale image.
Igray(Igray<0) = 0;
Igray(Igray>1) = 1;

% Create an index image.
Iidx = round(Igray * (size(clrmap, 1)-1) + 1);

% Generate color image from the gray scale one.
[M, N] = size(Igray);
Iclr = zeros(M, N, 3);
for i = 1:3
  clrmap_i = clrmap(:,i);
  Iclr_i = zeros(M, N);
  Iclr_i(isfinite(Igray)) = clrmap_i(Iidx(isfinite(Igray)));
  Iclr_i(isnan(Igray)) = options.NanClr(i);
  Iclr(:,:,i) = Iclr_i;
end
