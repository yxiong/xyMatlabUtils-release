"""
" Generate template source files (xxx.h, xxx.cc and xxxTest.cc).
"
" Usage example:
"   python generate_template_sources.py SourceFileName
"
"   Author: Ying Xiong.
"   Created: Aug 23, 2012.
"""

import datetime
import sys

today_str = datetime.datetime.today().strftime("%b %d, %Y")

topTemplate = """/**
  * Description.
  *
  * Author: Ying Xiong.
  * Created: """ + today_str + """.
  */

"""

namespace_str = """namespace xyUtils  {
}   // namespace xyUtils"""

testTemplate = """
#include "LogAndCheck.h"
#include "Timer.h"

using namespace xyUtils;

int main()  {
  Timer timer;
  LOG(INFO) << "Test on ...";

  LOG(INFO) << "Passed. [" << timer.elapsed() << " seconds]";
  return 0;
}"""

def GetHeaderMacro(filename):
  text = "__XYUTILS_"
  start = 0
  end = 1
  while (end <= len(filename)):
    if (end==len(filename) or filename[end].isupper()):
      text += filename[start:end].upper() + '_'
      start = end
    end += 1
  text += "H__"
  return text

if __name__ == "__main__":
  if (len(sys.argv) != 2):
    print(__doc__)
    sys.exit(1)
  filename = sys.argv[1]

  hfilename = filename + ".h"
  hfile = open(hfilename, "w")
  header_macro = GetHeaderMacro(filename)
  hfile.write(topTemplate)
  hfile.write("#ifndef %s\n" % header_macro)
  hfile.write("#define %s\n\n" % header_macro)
  hfile.write(namespace_str)
  hfile.write("\n\n#endif   // " + header_macro)
  hfile.close()
  
  ccfile = open(filename + ".cc", "w")
  ccfile.write(topTemplate)
  ccfile.write("#include \"%s\"\n\n" % hfilename)
  ccfile.write(namespace_str)
  ccfile.close()

  testfile = open(filename + "Test.cc", "w")
  testfile.write(topTemplate)
  testfile.write("#include \"%s\"\n" % hfilename)
  testfile.write(testTemplate)
  testfile.close()
