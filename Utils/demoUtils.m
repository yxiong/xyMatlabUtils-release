% Run all the demo scripts in current directory.
%
%   Author: Ying Xiong.
%   Created: Jan 20, 2014.

curPath = fileparts(mfilename('fullpath'));
demoScripts = dir(fullfile(curPath, 'demo*.m'));

for i = 1:length(demoScripts)
  if (~strcmp(demoScripts(i).name, [mfilename, '.m']))   % not this file.
    fprintf('Running %s...\n', demoScripts(i).name);
    run(demoScripts(i).name);
  end
end
