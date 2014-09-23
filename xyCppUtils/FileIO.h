/**
  * File I/O utilities.
  *
  * Author: Ying Xiong.
  * Created: Apr 30, 2013.
  */

#ifndef __XYUTILS_FILE_IO_H__
#define __XYUTILS_FILE_IO_H__

#include <cstdio>
#include <string>

namespace xyUtils  {
namespace FileIO {

// Read the content of the file 'filename' into a single string.
std::string ReadWholeFileToString(const char* filename);

// Read the rest of file 'fp' into a single string.
std::string ReadRestOfFileToString(FILE* fp);

// Read one line of the file 'fp' into a string. The return value will contain
// at least an '\n' character if the read is successful, and will be an empty
// string if reached the end of the file. Following code fragment read and
// process the file line by line:
//
//   FILE* fp = fopen("/path/to/file", "r");
//   if (fp == NULL) {
//     /* Do something, probably a LOG(FATAL). */
//   }
//   std::string line = FileIO::ReadLineToString(fp);
//   while (!line.empty()) {
//     /* Some code to process 'line'. */
//     line = FileIO::ReadLineToString(fp);
//   }
//   fclose(fp);
std::string ReadLineToString(FILE* fp);
  
}   // namespace FileIO
}   // namespace xyUtils

#endif   // __XYUTILS_FILE_IO_H__
