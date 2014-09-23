/**
  * Logging and checking utilities.
  *
  * Author: Ying Xiong
  * Created: Aug 14, 2012
  */

#ifndef __XYUTILS_LOG_AND_CHECK_H__
#define __XYUTILS_LOG_AND_CHECK_H__

#include <cstring>
#include <ostream>
#ifdef __MATLAB__  // Note: use -D__MATLAB__ when compiling for matlab.
#include "mex.h"
#include <sstream>
#endif

/** Prints message to stdout or stderr.
  * Example:
  *   LOG(PLAIN) << "Text message.\n";  // Prints to stdout.
  *   LOG(INFO) << "Log message.";      // Prints to stdout with file name and
  *                                     // line number.
  *   LOG(ERROR) << "Error message.";   // Prints to stderr with file name and
  *                                     // line number.
  *   LOG(FATAL) << "Error message.";   // Prints to stderr with file name and
  *                                     // line number, and exits the program.
  */
#define LOG(severity)                                                     \
  xyUtils::__LogAndCheck__::Log(xyUtils::__LogAndCheck__::severity,       \
      (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__),   \
      __LINE__)

/** Prints an error message and exits the program if the 'condition' failed.
  * Example:
  *   CHECK(1 >= 0);    // Nothing happens.
  *   CHECK(1 == 0);    // Prints an error message and exits the program.
  */
#define CHECK(condition)                                                  \
  if (!(condition)) {                                                     \
    LOG(FATAL) << "Check failed: " << #condition << ".";                  \
  }

/** Prints an error message and exits the program if the comparison between
  * 'obj1' and 'obj2' failed.
  * Example:
  *   CHECK_GE(1, 0);   // Nothing happens.
  *   CHECK_LE(1, 0);   // Prints an error message and exits the program.
  */
#define CHECK_EQ(obj1, obj2) __CHECK_OP__(==, obj1, obj2)
#define CHECK_NE(obj1, obj2) __CHECK_OP__(!=, obj1, obj2)
#define CHECK_GE(obj1, obj2) __CHECK_OP__(>=, obj1, obj2)
#define CHECK_LE(obj1, obj2) __CHECK_OP__(<=, obj1, obj2)
#define CHECK_GT(obj1, obj2) __CHECK_OP__(>, obj1, obj2)
#define CHECK_LT(obj1, obj2) __CHECK_OP__(<, obj1, obj2)

/** Prints an error message and exits the program if the 'obj1' and 'obj2' are
  * not near to each other within 'tol'.
  */
#define CHECK_NEAR(obj1, obj2, tol) {                                     \
  CHECK_LE(obj1, obj2 + tol);                                             \
  CHECK_GE(obj1, obj2 - tol);                                             \
}

/** Debug version of CHECK series, only fires when __DEBUG__ flag is specified
  * to have better efficiency in release mode.
  * Example:
  *   DCHECK_LT(1, 0);  // If __DEBUG__ flag is specified, prints an error
  *                     // message and exits the program, otherwise nothing
  *                     // happens.
  */
#ifdef __DEBUG__
#define DCHECK(condition) CHECK(condition)
#define DCHECK_EQ(obj1, obj2) CHECK_EQ(obj1, obj2)
#define DCHECK_NE(obj1, obj2) CHECK_NE(obj1, obj2)
#define DCHECK_GE(obj1, obj2) CHECK_GE(obj1, obj2)
#define DCHECK_LE(obj1, obj2) CHECK_LE(obj1, obj2)
#define DCHECK_GT(obj1, obj2) CHECK_GT(obj1, obj2)
#define DCHECK_LT(obj1, obj2) CHECK_LT(obj1, obj2)
#define DCHECK_NEAR(obj1, obj2, tol) CHECK_NEAR(obj1, obj2, tol)
#else
#define DCHECK(condition) (void) 0
#define DCHECK_EQ(obj1, obj2) (void) 0
#define DCHECK_NE(obj1, obj2) (void) 0
#define DCHECK_GE(obj1, obj2) (void) 0
#define DCHECK_LE(obj1, obj2) (void) 0
#define DCHECK_GT(obj1, obj2) (void) 0
#define DCHECK_LT(obj1, obj2) (void) 0
#define DCHECK_NEAR(obj1, obj2, tol) (void) 0;
#endif

//==========================================================
// Private sector.
//==========================================================
#define __CHECK_OP__(op, obj1, obj2)   {                                  \
  __typeof__ (obj1) _obj1 = (obj1);                                       \
  __typeof__ (obj2) _obj2 = (obj2);                                       \
  if (!(_obj1 op _obj2))   {                                              \
    LOG(ERROR) << "Check failed: "                                        \
               << #obj1 << " " << #op << " " << #obj2 << ".";             \
    LOG(FATAL) << "Actual value: " << _obj1 << " v.s. " << _obj2 << ".";  \
  }                                                                       \
}

namespace xyUtils  {
namespace __LogAndCheck__ {
enum LogSeverity  {
  PLAIN, INFO, ERROR, FATAL
};

class Log {
 public:
  // Constructor prints the file and line information.
  Log(LogSeverity severity, const char* file, int line);
  // Destructor prints the endl and exits the program if necessary.
  ~Log();
  // Forward the print operation to underlying stream.
  template<typename T> Log& operator<<(const T& data) {
#ifdef __MATLAB__
    std::stringstream ss;
    ss << data;
    mexPrintf("%s", ss.str().c_str());
#else
    *out_stream_ << data;
#endif
    return *this;
  }
  // Used to deal with endl, which is itself a function handle.
  friend Log& operator<<(Log& log, std::ostream& (*pf)(std::ostream&));
 private:
  LogSeverity severity_;
  std::ostream* out_stream_;
};
Log& operator<<(Log& log, std::ostream& (*pf)(std::ostream&));

}   // namespace __LogAndCheck__
}   // namespace xyUtils

#endif  // __XYUTILS_LOG_AND_CHECK_H__
