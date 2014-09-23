% Run all demo scripts for 'xyMatlabUtils'.
%
%   Author: Ying Xiong.
%   Created: Jan 19, 2014.

this_file_path = fileparts(mfilename('fullpath'));
folders = dir(this_file_path);

for iFolder = 1:length(folders)
  if (folders(iFolder).isdir && ...
      ~strcmp(folders(iFolder).name, '.') && ...
      ~strcmp(folders(iFolder).name, '..'))
    demo_script = ['demo' folders(iFolder).name '.m'];
    if (exist(fullfile(folders(iFolder).name, demo_script), 'file'))
      fprintf('================ Running %s ================\n', demo_script);
      cd(folders(iFolder).name);
      run(demo_script);
      drawnow;
      cd('..');
    end
  end
end
