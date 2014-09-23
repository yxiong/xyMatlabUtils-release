/**
  * Logging and checking utilities.
  *
  * Author: Ying Xiong
  * Created: Aug 14, 2012
  */

#include"LogAndCheck.h"

#include"stdlib.h"

#include<iostream>
#include<ostream>

#ifdef __MATLAB__
#include "mex.h"
#endif

using std::ostream;

namespace xyUtils  {
namespace __LogAndCheck__ {
Log::Log(LogSeverity severity, const char* file, int line)
    : severity_(severity) {
  switch (severity) {
    case PLAIN:
    case INFO:
      out_stream_ = &std::cout;
      break;
    case ERROR:
    case FATAL:
      out_stream_ = &std::cerr;
      break;
    default:
      std::cerr << "Internal Error: Unknown LogSeverity!\n";
      exit(1);
  }
  if (severity != PLAIN) {
#ifdef __MATLAB__
    mexPrintf("[%s:%d] ", file, line);
#else
    *out_stream_ << "[" << file << ": " << line << "] ";
#endif
  }
}

Log::~Log() {
  if (severity_ != PLAIN)  {
#ifdef __MATLAB__
    mexPrintf("\n");
#else
    *out_stream_ << std::endl;
#endif
  }
  if (severity_ == FATAL)  {
    // TODO: print stack trace.
#ifdef __MATLAB__
    mexErrMsgTxt(""); // TODO.
#else
    exit(1);
#endif
  }
}

Log& operator<<(Log& log, ostream& (*pf)(ostream&))  {
  *log.out_stream_ << pf;
  return log;
}

}   // namespace __LogAndCheck__
}   // namespace xyUtils
