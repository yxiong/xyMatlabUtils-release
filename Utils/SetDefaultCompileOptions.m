function opts = SetDefaultCompileOptions(opts)

% options = SetDefaultCompileOptions()
% options = SetDefaultCompileOptions(options)
%
% Set default compile options struct. The input 'options' can have some fields
% set, and the output 'options' will have the rest set as default.
%
%  Fields         Default           Description
%  ================================================================
%  'cc'           'gcc'             C compiler.
%  'cxx'          'g++'             C++ compiler.
%  'ld'           same as 'cxx'     Linker.
%
%   Author: Ying Xiong.
%   Created: May 18, 2014.

if (~exist('opts', 'var'))               opts = struct();                end

if (~isfield(opts, 'cc'))                opts.cc = 'gcc';                end
if (~isfield(opts, 'cxx'))               opts.cxx = 'g++';               end
if (~isfield(opts, 'ld'))                opts.ld = opts.cxx;             end
