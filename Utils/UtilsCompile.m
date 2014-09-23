function UtilsCompile(opts)

% UtilsCompile(opts)
%
% Compile all the .cc files in this directory.
%
%   Author: Ying Xiong.
%   Created: Mar 29, 2014.

% Set default options.
if (~exist('opts', 'var'))   opts = struct();   end
opts = SetDefaultCompileOptions(opts);

% Get all the .cc files in this directory.
thispath = fileparts(mfilename('fullpath'));
ccFiles = dir(fullfile(thispath, '*.cc'));

% Library paths.
xyCppUtilsPath = fullfile(thispath, '..', 'xyCppUtils');
xyMexUtilsPath = fullfile(thispath, '..', 'mexUtils');

for i = 1:length(ccFiles)
  fprintf('Compiling %s...\n', ccFiles(i).name);
  mexCmd = ['mex CXX="' opts.cxx '"' ...
            ' LD="' opts.ld '"' ...
            ' -I' xyCppUtilsPath ' -L' xyCppUtilsPath ' -lxyutils' ...
            ' -I' fullfile(xyCppUtilsPath, 'ThirdParty/include') ...
            ' -I' xyMexUtilsPath ' -L' xyMexUtilsPath ' -lxymex' ...
            ' -D__MATLAB__' ...
            ' ' fullfile(thispath, ccFiles(i).name) ...
            ' -outdir ' thispath];
  eval(mexCmd);
end
