/**
  * Test for LogAndCheck.
  *
  * Author: Ying Xiong
  * Created: Aug 14, 2012
  */

#include"LogAndCheck.h"

#include<string>

using std::string;
using namespace xyUtils;

int main()  {
  // Test logging.
  LOG(PLAIN) << "Plain text1...";
  LOG(PLAIN) << " (continue)\n";
  LOG(INFO) << "Print log info...";
  LOG(ERROR) << "Print error info...";
  // LOG(FATAL) << "Print error info and kill the program!";

  // Test cheching.
  CHECK("Pass");
  CHECK_EQ(1, 2-1);
  CHECK_EQ(string("Hello"), "Hello");   // Should success.
  CHECK_GT(2+2, 3);
  CHECK_LT(2+2, 5);
  CHECK_GE(3, 3);
  CHECK_LE(1, 3);

  // Following check will fail and exit the program.
  CHECK_EQ(string("Intentionally"), "Fail");   // Should success.
  LOG(INFO) << "Shouldn't see this.";

  return 0;
}
