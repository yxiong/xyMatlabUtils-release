/**
  * Utility functions for std::string.
  *
  * Author: Ying Xiong.
  * Created: May 04, 2013.
  */

#ifndef __XYUTILS_STRING_UTILS_H__
#define __XYUTILS_STRING_UTILS_H__

#include <string>
#include <vector>

namespace xyUtils  {
namespace StringUtils {

enum CommentStyle {
  CComment,       // Everything between /* and */, both inclusive.
  CppComment,     // Everything between // and \n, including // but not \n.
                  // plus CComment.
  ShellComment    // Everything between # and \n, including # but not \n.
};

// Return a list of the words in the string 'str', using whitespace as the
// delimiter. Empty strings are removed from the results.
std::vector<std::string> Split(const char* str);
inline std::vector<std::string> Split(const std::string& str) {
  return Split(str.c_str());
}

// Return a list of the words in the string 'str', using 'sep' as the delimiter.
// If maxSplit >= 0, at most 'maxSplit' splits are done, default {-1}.
// If 'discardRest' is true, the size of result vector is at most maxSplit+1 with
// the last element the rest of the string not splitted yet (if any);
// If 'discardRest' is false, the size of result vector is at most maxSplit, and
// the rest of the string is discarded. Default {false}.
std::vector<std::string> Split(const char* str, char sep,
                               int maxSplit=-1, bool discardRest=false);
std::vector<std::string> Split(const char* str, const char* sep,
                               int maxSplit=-1, bool discardRest=false);
template<typename T>
std::vector<std::string> Split(const std::string& str, const T& sep,
                               int maxSplit=-1, bool discardRest=false) {
  return Split(str.c_str(), sep, maxSplit, discardRest);
}

// Strip the white space in front and at the end of the string.
// E.g. "\t  abc def 123 456  \t  \n" ===> "abc def 123 456"
void StripWhiteSpace(std::string* str);

// Strip all text in comments.
void StripComments(std::string* str, CommentStyle style);

// Print formatted data to a string. This function is safe (no memory leak or
// overflow), but not necessarily efficient.
std::string PrintfToString(const char* format, ...);

}   // namespace StringUtils
}   // namespace xyUtils

#endif   // __XYUTILS_STRING_UTILS_H__
