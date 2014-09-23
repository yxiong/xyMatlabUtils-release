function xyMatlabUtilsCompile(opts)

% xyMatlabUtilsCompile()
% xyMatlabUtilsCompile(options)
%
% Run all compile functions in 'xyMatlabUtils'. See 'SetDefaultCompileOptions.m'
% for argument 'options'.
%
%   Author: Ying Xiong.
%   Created: Sep 23, 2014.

%% Setup default options.
if (~exist('opts', 'var'))   opts = struct();   end
opts = SetDefaultCompileOptions(opts);

%% Compile 'xyCppUtis'.
fprintf('================ Compile "xyCppUtils" ================\n');
cd('xyCppUtils');
[~, uname] = system('uname');
uname = uname(1:end-1);   % Remove newline.
config_cmd = ['python configure.py ' ...
              ' --no-external --no-third-party --with-eigen' ...
              ' --CC="' opts.cc '" --CXX="' opts.cxx '"'];
status = system(config_cmd);
assert(status == 0);
status = system('make clean');
assert(status == 0);
status = system(['make MATLAB=1 MATLAB_ROOT=', matlabroot, ...
                 ' RELEASE=1 libxyutils.a']);
assert(status == 0);
cd('..');

%% Compile 'MexUtils'.
fprintf('================ Running MexUtils ================\n');
cd('MexUtils');
eval('MexUtilsCompile(opts)');
cd('..');

%% Compile other scripts.

this_file_path = fileparts(mfilename('fullpath'));
folders = dir(this_file_path);

for iFolder = 1:length(folders)
  if (folders(iFolder).isdir && ...
      ~strcmp(folders(iFolder).name, '.') && ...
      ~strcmp(folders(iFolder).name, '..') && ...
      ~strcmp(folders(iFolder).name, 'MexUtils'))
    compile_script = [folders(iFolder).name 'Compile.m'];
    if (exist(fullfile(folders(iFolder).name, compile_script), 'file'))
      fprintf('================ Running %s ================\n', compile_script);
      cd(folders(iFolder).name);
      run(compile_script);
      cd('..');
    end
  end
end
