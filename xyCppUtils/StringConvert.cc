/**
  * Utilities for converting string (char*) to other various data types.
  *
  * Author: Ying Xiong
  * Created: Aug 18, 2012
  */

#include "StringConvert.h"

#include <cerrno>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <cstring>

using std::string;

namespace xyUtils  {
namespace StringConvert {

std::string OutStatusString(OutStatus status) {
  switch (status) {
    case Success:           return "Success";
    case Overflow:          return "Overflow";
    case Underflow:         return "Underflow";
    case Inconvertible:     return "Inconvertible";
    default:
      LOG(FATAL) << "Internal Error: Unknown OutStatus.";
      return "";
  }
}

OutStatus ToBool(const char* str, bool* b)  {
  if (strcmp(str, "True") == 0 ||
      strcmp(str, "true") == 0 ||
      strcmp(str, "1") == 0) {
    *b = true;
    return Success;
  } else if (strcmp(str, "False") == 0 ||
             strcmp(str, "false") == 0 ||
             strcmp(str, "0") == 0)  {
    *b = false;
    return Success;
  } else  {
    return Inconvertible;
  }
}

OutStatus ToInt(const char* str, int* i)  {
  char* end;
  errno = 0;
  long l = strtol(str, &end, 0);
  if ((errno == ERANGE && l == LONG_MAX) || l > INT_MAX)  {
    return Overflow;
  } else if ((errno == ERANGE && l == LONG_MIN) || l < INT_MIN) {
    return Underflow;
  } else if (*str == '\0' || *end != '\0')  {
    return Inconvertible;
  } else  {
    *i = l;
    return Success;
  }
}

OutStatus ToLong(const char* str, long* l)  {
  char* end;
  errno = 0;
  *l = strtol(str, &end, 0);
  if (errno == ERANGE && *l == LONG_MAX) {
    return Overflow;
  } else if (errno == ERANGE && *l == LONG_MIN)  {
    return Underflow;
  } else if (*str == '\0' || *end != '\0')  {
    return Inconvertible;
  } else  {
    return Success;
  }
}

OutStatus ToDouble(const char* str, double* d)  {
  char* end;
  errno = 0;
  *d = strtod(str, &end);
  if (errno == ERANGE && *d == HUGE_VAL)  {
    return Overflow;
  } else if (errno == ERANGE && *d == -HUGE_VAL)  {
    return Underflow;
  } else if (*str == '\0' || *end != '\0')  {
    return Inconvertible;
  } else  {
    return Success;
  }
}
}   // namespace StringConvert
}   // namespace xyUtils
