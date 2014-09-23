% Run all test scripts in 'xyMatlabUtils'.
%
%   Author: Ying Xiong.
%   Created: Jan 19, 2014.

this_file_path = fileparts(mfilename('fullpath'));
folders = dir(this_file_path);

for iFolder = 1:length(folders)
  if (folders(iFolder).isdir && ...
      ~strcmp(folders(iFolder).name, '.') && ...
      ~strcmp(folders(iFolder).name, '..'))
    test_script = [folders(iFolder).name 'Test.m'];
    if (exist(fullfile(folders(iFolder).name, test_script), 'file'))
      fprintf('================ Running %s ================\n', test_script);
      cd(folders(iFolder).name);
      run(test_script);
      cd('..');
    end
  end
end
