/**
  * Utilities for converting string (char*) to various other data types.
  *
  * Author: Ying Xiong
  * Created: Aug 18, 2012
  */

#ifndef __XYUTILS_STRING_CONVERTER_H__
#define __XYUTILS_STRING_CONVERTER_H__

#include <string>

#include "LogAndCheck.h"

namespace xyUtils  {
namespace StringConvert {
enum OutStatus  {
  Success, Overflow, Underflow, Inconvertible
};

std::string OutStatusString(OutStatus);

/** Convert char* string to bool/int/long/double.
 * bool type supports "True", "true", "1", "False", "false" and "0".
 */
OutStatus ToBool(const char* str, bool* b);
OutStatus ToInt(const char* str, int* i);
OutStatus ToLong(const char* str, long* l);
OutStatus ToDouble(const char* str, double* d);

inline OutStatus ToBool(const std::string& str, bool* b) {
  return ToBool(str.c_str(), b);
}
inline OutStatus ToInt(const std::string& str, int* i) {
  return ToInt(str.c_str(), i);
}
inline OutStatus ToLong(const std::string& str, long* l) {
  return ToLong(str.c_str(), l);
}
inline OutStatus ToDouble(const std::string& str, double* d) {
  return ToDouble(str.c_str(), d);
}

template<typename T> int ToBoolAndCheck(const T& str) {
  bool b;
  OutStatus out = ToBool(str, &b);
  if (out != Success) {
    LOG(FATAL) << "Error: " << OutStatusString(out)
               << ": Cannot convert string \"" << str << "\" to bool.";
  }
  return b;
}

template<typename T> int ToIntAndCheck(const T& str) {
  int i;
  OutStatus out = ToInt(str, &i);
  if (out != Success) {
    LOG(FATAL) << "Error: " << OutStatusString(out)
               << ": Cannot convert string \"" << str << "\" to int.";
  }
  return i;
}

template<typename T> long ToLongAndCheck(const T& str) {
  long l;
  OutStatus out = ToLong(str, &l);
  if (out != Success) {
    LOG(FATAL) << "Error: " << OutStatusString(out)
               << ": Cannot convert string \"" << str << "\" to long.";
  }
  return l;
}

template<typename T> double ToDoubleAndCheck(const T& str) {
  double d;
  OutStatus out = ToDouble(str, &d);
  if (out != Success) {
    LOG(FATAL) << "Error: " << OutStatusString(out)
               << ": Cannot convert string \"" << str << "\" to double.";
  }
  return d;
}
}   // namespace StringConvert
}   // namespace xyUtils

#endif    // __XYUTILS_STRING_CONVERTER_H__
