================================================================
xyMatlabUtils -- Matlab utilities by Ying Xiong.
================================================================

Author: Ying Xiong.
Created: Nov 17, 2011.
Release: Sep 23, 2014 (v0.0.8).

================================================================
Quick start.
================================================================
>> xyMatlabUtilsSetPath;    % Add path for children folders.
>> xyMatlabUtilsCompile;    % Compile the mex files.
>> xyMatlabUtilsTest;       % Run all the test scripts.
>> xyMatlabUtilsDemo;       % Run all the demo scripts.

================================================================
Naming conventions.
================================================================

The toolboxes are named something like 'XYZBox', with 'XYZ' the acronym of the
toolbox. Each toolbox should be as independent to others as possible. Each
toolbox should contain a 'XYZBoxTest.m' and a 'demoXYZBox.m' script, for testing
and demonstration use.

Public funtions are named something like 'PublicFunctionNames.m', with each word
capitalized and (at the best effort) spelled out completely.

Public scripts are named something like 'doThisStuff.m', with first word a verb
and not capitalized followed by other words capitalized.

Private functions for specific toolbox are named something like 'XYZFuncName',
with prefix 'XYZ' the acronym of the toolbox, and 'FuncName' the function name
whose words can also be abbreviated.

Most functions come with a test script with name 'FunctionNameTest.m'.
