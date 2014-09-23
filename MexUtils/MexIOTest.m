function MexIOTest()

%   Author: Ying Xiong.
%   Created: Sep 23, 2014.

InvalidArgTest1();
InvalidArgTest2();
InvalidArgTest3();

ValidArgTest();
fprintf('Passed.\n');
end


function InvalidArgTest1()

% Invalid number of arguments.
try
  MexIOTestHelper();
catch
  return;
end
assert(0, 'Should never get here.');

end

function InvalidArgTest2()

% Non-double input.
try
  MexIOTestHelper('abc', 'def', '1234', '456', 'str');
catch
  return;
end
assert(0, 'Should never get here.');

end

function InvalidArgTest3()

% Non-string input.
try
  MexIOTestHelper(1, 2, 3, 4, 5);
catch
  return;
end
assert(0, 'Should never get here.');

end

function ValidArgTest()

d = 256;
v = [1; 2; 3; 4];
m = randn(4, 5);
md = randn(2, 3, 4);
str = 'Hello World!';

[d2, v2, m2, md2, str2] = MexIOTestHelper(d, v, m, md, str);

assert(d2 == d + 42);
assert(all(abs(v2 - v - 42) < eps));

assert(all(size(m2) == size(m)));
assert(all(abs(m2(:) - m(:) - 42) < eps));

assert(all(size(md2) == size(md)));
assert(all(abs(md2(:) - md(:) - 42) < eps));

assert(strcmp(str2, '!dlroW olleH'));

end
