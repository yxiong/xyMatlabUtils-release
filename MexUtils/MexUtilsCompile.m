function MexUtilsCompile(opts)

% MexUtilsCompile()
% MexUtilsCompile(options)
%
% Compile the library. See 'SetDefaultCompileOptions.m' for argument 'options'.
%
%   Author: Ying Xiong.
%   Created: Dec 16, 2012.
%   Updated: May 18, 2014,
%            Sep 23, 2014.

% Setup default options.
if (~exist('opts', 'var'))   opts = struct();   end
opts = SetDefaultCompileOptions(opts);

% Compile the library.
fprintf('**** make clean ****\n');
status = system('make clean');
assert(status == 0);

fprintf('**** make ****\n');
status = system(['make' ...
                 ' CC=' opts.cc ...
                 ' CXX=' opts.cxx ...
                 ' MATLAB_ROOT=' matlabroot]);
assert(status == 0);
