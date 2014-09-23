/**
  * Utility functions for std::string.
  *
  * Author: Ying Xiong.
  * Created: May 04, 2013.
  */

#include "StringUtils.h"

#include <cstdarg>
#include <cstring>
#include <string>
#include <vector>

#include "LogAndCheck.h"

using namespace std;
namespace xyUtils  {
namespace StringUtils {

vector<string> Split(const char* str) {
  vector<string> parts;
  bool newPart = true;
  for (const char* s = str; *s != 0; ++s) {
    if (isspace(*s)) {
      newPart = true;
    } else {
      if (newPart)        parts.push_back(string());
      parts.back().push_back(*s);
      newPart = false;
    }
  }
  return parts;
}

vector<string> Split(const char* str, char sep,
                               int maxSplit, bool discardRest) {
  vector<string> parts;
  bool newPart = true;
  const char* s;
  for (s = str; *s != 0; ++s) {
    if (newPart) {
      newPart = false;
      if (int(parts.size()) == maxSplit) break;
      parts.push_back(string());
    }
    if (*s == sep) {
      newPart = true;
    } else {
      parts.back().push_back(*s);
    }
  }
  if (int(parts.size())==maxSplit && !discardRest) {
    parts.push_back(string());
    for (; *s != 0; ++s) {
      parts.back().push_back(*s);
    }
  } else {
    if (newPart) parts.push_back(string());
  }
  return parts;
}

vector<string> Split(const char* str, const char* sep,
                               int maxSplit, bool discardRest) {
  vector<string> parts;
  bool newPart = true;
  const char* s;
  size_t sepLen = strlen(sep);
  for (s = str; *s != 0; ++s) {
    if (newPart) {
      newPart = false;
      if (int(parts.size()) == maxSplit) break;
      parts.push_back(string());
    }
    if (strncmp(s, sep, sepLen) == 0) {
      newPart = true;
      s += sepLen-1;
    } else {
      parts.back().push_back(*s);
    }
  }
  if (int(parts.size())==maxSplit && !discardRest) {
    parts.push_back(string());
    for (; *s != 0; ++s) {
      parts.back().push_back(*s);
    }
  } else {
    if (newPart) parts.push_back(string());
  }
  return parts;
}

void StripWhiteSpace(string* str) {
  int i;
  for (i = 0; i < int(str->size()); ++i) {
    if (!isspace(str->at(i))) break;
  }
  if (i == int(str->size())) {
    str->clear();
    return;
  } else if (i > 0) {
    str->erase(0, i);
  }

  for (i = str->size()-1; i >= 0; --i) {
    if (!isspace(str->at(i))) break;
  }
  CHECK_GT(i, 0);
  str->resize(i+1);
}

static void StripCommentsHelper(string* str,
                                const string& startTag,
                                const string& endTag,
                                bool startInclude,
                                bool endInclude) {
  size_t startPos = str->find(startTag) + (startInclude ? 0 : startTag.size());
  while (startPos != string::npos) {
    size_t endPos = str->find(endTag);
    if (endPos == string::npos) {
      if (endInclude) {
        LOG(FATAL) << "Incomplete comment in string\n" << *str;
      } else {
        str->resize(startPos);
      }
    } else {
      str->erase(startPos, endPos - startPos + (endInclude ? endTag.size() : 0));
    }
    startPos = str->find(startTag, startPos);
  }
}
                                

void StripComments(string* str, CommentStyle style) {
  switch (style) {
    case CComment:
      StripCommentsHelper(str, "/*", "*/", true, true);
      break;
    case CppComment:
      StripCommentsHelper(str, "/*", "*/", true, true);
      StripCommentsHelper(str, "//", "\n", true, false);
      break;
    case ShellComment:
      StripCommentsHelper(str, "#", "\n", true, false);
      break;
    default:
      LOG(FATAL) << "Internal error: should not get here!";
  }
}

string PrintfToString(const char* format, ...) {
  // Get number of characters that should be written.
  va_list args;
  va_start(args, format);
  int nWrite = vsnprintf(NULL, 0, format, args);
  CHECK(nWrite >= 0);
  va_end(args);
  // Allocate memeory and print the string.
  char* tmp = new char[nWrite+1];
  va_start(args, format);
  nWrite = vsnprintf(tmp, nWrite+1, format, args);
  va_end(args);
  string str(tmp);
  delete[] tmp;
  return str;
}
}   // namespace StringUtils
}   // namespace xyUtils
