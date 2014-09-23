/**
  * Test for file I/O utilities.
  *
  * Author: Ying Xiong.
  * Created: Apr 30, 2013.
  */

#include "FileIO.h"

#include <cstdio>
#include <string>

#include "LogAndCheck.h"
#include "Timer.h"

using namespace xyUtils;

int main()  {
  Timer timer;
  LOG(INFO) << "Test on FileIO ...";

  const char* filename = "TestData/Texts/FileIOTestFile.txt";
  // Read the whole file.
  std::string texts = FileIO::ReadWholeFileToString(filename);
  // Read the file line by line and check if get the same result.
  FILE* fp = fopen(filename, "r");
  std::string line = FileIO::ReadLineToString(fp);
  size_t index = 0;
  while (!line.empty()) {
    size_t len = line.size();
    CHECK_EQ(line, texts.substr(index, len));
    index += len;
    line = FileIO::ReadLineToString(fp);    
  }
  CHECK_EQ(index, texts.size());
  fclose(fp);

  LOG(INFO) << "Passed. [" << timer.elapsed() << " seconds]";
  return 0;
}
