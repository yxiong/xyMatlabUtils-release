/**
  * Test for StringUtils functions.
  *
  * Author: Ying Xiong.
  * Created: May 04, 2013.
  */

#include "StringUtils.h"

#include <string>
#include <vector>

#include "LogAndCheck.h"
#include "Timer.h"

using namespace std;
using namespace xyUtils;

int main()  {
  Timer timer;
  LOG(INFO) << "Test on string utilities...";

  string str;
  // Test on Split.
  str = "\t abc  \t def\n\n123\n456   \t \n";
  vector<string> parts = StringUtils::Split(str);
  CHECK_EQ(parts.size(), 4);
  CHECK_EQ(parts[0], "abc");
  CHECK_EQ(parts[1], "def");
  CHECK_EQ(parts[2], "123");
  CHECK_EQ(parts[3], "456");

  str = "\t abc  # def##123\n456\n";
  parts = StringUtils::Split(str, '#');
  CHECK_EQ(parts.size(), 4);
  CHECK_EQ(parts[0], "\t abc  ");
  CHECK_EQ(parts[1], " def");
  CHECK_EQ(parts[2], "");
  CHECK_EQ(parts[3], "123\n456\n");
  parts = StringUtils::Split(str, '#', 2);
  CHECK_EQ(parts.size(), 3);
  CHECK_EQ(parts[0], "\t abc  ");
  CHECK_EQ(parts[1], " def");
  CHECK_EQ(parts[2], "#123\n456\n");
  parts = StringUtils::Split(str, '#', 2, true);
  CHECK_EQ(parts.size(), 2);
  CHECK_EQ(parts[0], "\t abc  ");
  CHECK_EQ(parts[1], " def");

  str = "\t abc  DEL defDELDEL123\n456\nDEL";
  parts = StringUtils::Split(str, "DEL");
  CHECK_EQ(parts.size(), 5);
  CHECK_EQ(parts[0], "\t abc  ");
  CHECK_EQ(parts[1], " def");
  CHECK_EQ(parts[2], "");
  CHECK_EQ(parts[3], "123\n456\n");
  CHECK_EQ(parts[4], "");

  // Test on StripWhiteSpace.
  str = "\t  abc def 123 456  \t  \n";
  StringUtils::StripWhiteSpace(&str);
  CHECK_EQ(str, "abc def 123 456");

  // Test on StripComments.
  str = "Test on /* bla bla\nbla */ C comments.\n";
  StringUtils::StripComments(&str, StringUtils::CComment);
  CHECK_EQ(str, "Test on  C comments.\n");

  str = "Test on /* bla bla\nbla */ Cpp // more bla\n comments.\n";
  StringUtils::StripComments(&str, StringUtils::CppComment);
  CHECK_EQ(str, "Test on  Cpp \n comments.\n");

  str = "Test on # comments\n Shell comments. # more comments.";
  StringUtils::StripComments(&str, StringUtils::ShellComment);
  CHECK_EQ(str, "Test on \n Shell comments. ");

  // Test on PrintfToString.
  str = StringUtils::PrintfToString("abc %d xyz %.02f %c %s",
                                    1, 2.34, 'p', "qrst");
  CHECK_EQ(str, string("abc 1 xyz 2.34 p qrst\0"));

  LOG(INFO) << "Passed. [" << timer.elapsed() << " seconds]";
  return 0;
}
