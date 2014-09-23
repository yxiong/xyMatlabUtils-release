/**
  * Test for CommandLineFlags.
  *
  * Author: Ying Xiong
  * Created: Jun 11, 2012
  */

#include "CommandLineFlags.h"

#include <iostream>

#include "LogAndCheck.h"

using namespace xyUtils;   // This is optional for command line flags.

DEFINE_FLAG_bool(help, false, "Print this help message.");
DEFINE_FLAG_bool(bool_test, false, "Test boolean flag.");
DEFINE_FLAG_char(char_test, 'c', "Test char flag.");
DEFINE_FLAG_int(int_test, 12, "Test int flag.");
DEFINE_FLAG_long(long_test, 12345678, "Test long flag.");
DEFINE_FLAG_double(double_test, 1234.56, "Test double flag.");
DEFINE_FLAG_string(string_test, "Hello World", "Test initialized string flag.");

int main(int argc, char** argv)  {
  CommandLineFlagsInit(&argc, &argv, true);
  if (FLAGS_help) {
    PrintCommandLineFlagsHelp();
    return 0;
  }

  LOG(INFO) << "Option flags:";
  LOG(INFO) << "FLAGS_bool_test = " << FLAGS_bool_test;
  LOG(INFO) << "FLAGS_char_test = " << FLAGS_char_test;
  LOG(INFO) << "FLAGS_int_test = " << FLAGS_int_test;
  LOG(INFO) << "FLAGS_long_test = " << FLAGS_long_test;
  LOG(INFO) << "FLAGS_double_test = " << FLAGS_double_test;
  LOG(INFO) << "FLAGS_string_test = " << FLAGS_string_test;

  LOG(INFO) << "Remaining arguments:";
  for (int i = 1; i < argc; ++i)  {
    LOG(INFO) << argv[i];
  }

  return 0;
}
