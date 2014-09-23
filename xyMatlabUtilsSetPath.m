function xyMatlabUtilsSetPath()

% xyMatlabUtilsSetPath()
%
% Set path for 'xyMatlabUtils' package.
%
%   Author: Ying Xiong.
%   Created: Jan 19, 2014.

this_file_path = fileparts(mfilename('fullpath'));

subPaths = {...
  'DfGBox', ...
  'NLOBox', ...
  'PSBox', ...
  'SHBox', ...
  'QBox', ...
  'Utils', ...
  'UnitTestUtils', ...
  'BoundingBox', ...
};

for i = 1:length(subPaths)
  addpath(fullfile(this_file_path, subPaths{i}));
end
