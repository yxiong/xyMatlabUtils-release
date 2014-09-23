/**
  * File I/O utilities.
  *
  * Author: Ying Xiong.
  * Created: Apr 30, 2013.
  */

#include "FileIO.h"

#include <cstdio>
#include <cstring>
#include <string>

#include "LogAndCheck.h"

namespace xyUtils  {
namespace FileIO {

std::string ReadWholeFileToString(const char* filename) {
  FILE* fp = fopen(filename, "r");
  if (fp == NULL) {
    perror(filename);
    LOG(FATAL) << "Error on opening file " << filename;
  }
  return ReadRestOfFileToString(fp);
}

std::string ReadRestOfFileToString(FILE* fp) {
  CHECK(fp);
  const int kBufSize = 4096;
  char buffer[kBufSize];
  std::string str;
  int nRead;
  while ((nRead = fread(buffer, 1, kBufSize, fp)) > 0) {
    if (ferror(fp)) {
      perror(NULL);
      LOG(FATAL) << "Error on reading file.";
    }
    str.append(buffer, nRead);
  }
  fclose(fp);
  return str;
}

std::string ReadLineToString(FILE* fp) {
  CHECK(fp);
  const size_t kBufSize = 1024;
  char buffer[kBufSize];
  std::string str;
  do {
    if (fgets(buffer, kBufSize, fp) == NULL) {
      // Error or reach end of file.
      if (ferror(fp)) {
        perror(NULL);
        LOG(FATAL) << "Error on reading file.";
      } else {
        CHECK(feof(fp));
        return str;
      }
    }
    str.append(buffer, strlen(buffer));
  } while ((strlen(buffer) == kBufSize) && (buffer[kBufSize-2] != '\n'));
  return str;
}

}   // namespace FileIO
}   // namespace xyUtils
