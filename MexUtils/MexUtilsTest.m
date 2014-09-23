% Run all the tests in current directory.
%
%   Author: Ying Xiong.
%   Created: Sep 23, 2014.

%% Compile the .cc files.

ccFiles = {'MexIOTestHelper.cc'};

% NOTE: 'thispath' and 'xyMexUtilsPath' can can be different if the .cc files
% to be compiled are in another folder.
thispath = fileparts(mfilename('fullpath'));
xyMexUtilsPath = thispath;
opts = SetDefaultCompileOptions();

for i = 1:length(ccFiles)
  fprintf('Compiling %s...\n', ccFiles{i});
  mexCmd = ['mex ' ...
            ' CC=' opts.cc ' CXX=' opts.cxx ' LD=' opts.ld ...
            ' -I' xyMexUtilsPath ' -L' xyMexUtilsPath ' -lxymex' ...
            ' ' fullfile(thispath, ccFiles{i}) ...
            ' -outdir ' thispath];
  eval(mexCmd);
end

%% Run all tests.
curPath = fileparts(mfilename('fullpath'));
testScripts = dir(fullfile(curPath, '*Test.m'));

for i = 1:length(testScripts)
  if (~strcmp(testScripts(i).name, [mfilename, '.m']))   % not this file.
    fprintf('Running %s...\n', testScripts(i).name);
    run(testScripts(i).name);
  end
end
