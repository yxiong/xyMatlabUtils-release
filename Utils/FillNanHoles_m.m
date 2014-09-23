function D = FillNanHoles_m(D, fillDirections, fillScales)

%   Author: Ying Xiong.
%   Created: Nov 12, 2013.

sz = size(D);
% The filling directions and scales with which we take average.
if (~exist('fillDirections', 'var'))
  fillDirections = [1 -1 0  0; ...
                    0  0 1 -1];
else
  assert(size(fillDirections, 1) == 2);
end
nDirections = size(fillDirections, 2);

if (~exist('fillScales', 'var'))
  fillScales = ones(nDirections, 1);
else
  assert(isvector(fillScales) && length(fillScales)==nDirections);
end

% Indices to be filled, length nNan.
nanIdx = find(isnan(D));
while (~isempty(nanIdx))
  %% Set up indicies and aggregation containers.
  % Sub-indices to be filled, length nNan.
  [i, j] = ind2sub(sz, nanIdx);
  nNan = length(nanIdx);
  % Aggregate numbers and values, length nNan.
  fillNum = zeros(nNan, 1);
  fillVal = zeros(nNan, 1);
  %% Aggregate the filling.
  % Index propagation: nanIdx -> vIdx -> nbrIdx.
  for iD = 1:size(fillDirections, 2)
    % Filling sub-indices, length nNan.
    iF = i+fillDirections(1,iD);
    jF = j+fillDirections(2,iD);
    % The indices for valid filling indices, length nValid.
    vIdx = find((iF>0) & (iF<=sz(1)) & (jF>0) & (jF<=sz(2)));
    % Filling indices, length nValid.
    idxF = sub2ind(sz, iF(vIdx), jF(vIdx));
    % The indices for valid neighbor, length and nNbr.
    nbrIdx = find(~isnan(D(idxF)));
    % Do the aggregation.
    fillVal(vIdx(nbrIdx)) = fillVal(vIdx(nbrIdx)) + ...
        D(idxF(nbrIdx)) * fillScales(iD);
    fillNum(vIdx(nbrIdx)) = fillNum(vIdx(nbrIdx)) + fillScales(iD);
  end
  %% Do the filling.
  D(nanIdx) = fillVal ./ fillNum;
  % Indices to be filled in next iteration.
  nanIdx = find(isnan(D));
end
