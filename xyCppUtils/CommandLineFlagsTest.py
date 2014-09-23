"""
"   Author: Ying Xiong.
"   Created: Aug 15, 2012.
"""

import os

if __name__ == "__main__":
  command = "./CommandLineFlagsTest \\\n" + \
            "--long_test=1234567890 arg1 --string_test=\"foo bar\" \\\n" + \
            "--int_test=0xff arg2 --bool_test \\\n" + \
            "--char_test=c --double_test=23.34 arg3"
  print(command)
  os.system(command)

  flagFile = os.path.join("TestData", "Texts", "CommandLineFlagsFileTest.txt")
  command = "./CommandLineFlagsTest \\\n" + \
            "--long_test=1234567890 arg1 arg2 arg3 \\\n" + \
            "--flagfile=" + flagFile + " arg4 \\\n" + \
            "--char_test=c arg5"
  print(command)
  os.system(command)
