/**
  * Command line flag utilities.
  *
  * A "flag" is essentially a static global variable 'FLAG_var' in the .cc file
  * that defines it, whose variable name and pointer stored in another static
  * variable 'get_static_register_flags()::__flags__' in this .cc
  * file. Therefore, the flag can only be accessed by the .cc file defined it
  * and the utilities in thie .cc file.
  *
  * Author: Ying Xiong.
  * Created: Jun 11, 2012.
  * Updated: Apr 30, 2013.
  */

#include "CommandLineFlags.h"

#include <algorithm>
#include <cstring>
#include <cstdio>
#include <cctype>
#include <string>
#ifdef __USE_TR1__
#include <tr1/unordered_map>
#else
#include <unordered_map>
#endif

#include "DataTypes.h"
#include "LogAndCheck.h"
#include "FileIO.h"
#include "StringUtils.h"
#include "StringConvert.h"

using std::string;
#ifdef __USE_TR1__
using std::tr1::unordered_map;
#else
using std::unordered_map;
#endif

namespace xyUtils  {
namespace __CommandLineFlags__  {

namespace {
struct __flag_type__ {
  __flag_type__()
      : dataTypePointer(), descrip(), setByUser(false), id(-1) {
  }
  __flag_type__(__data_type_pointer__ _dataTypePointer,
                const char* _descrip)
      : dataTypePointer(_dataTypePointer), descrip(_descrip), setByUser(false) {
    id = nFlags++;
  }
  __data_type_pointer__ dataTypePointer;
  string descrip;
  bool setByUser;
  int id;
  static int nFlags;
};
int __flag_type__::nFlags = 0;
} // namespace

static unordered_map<string, __flag_type__>& get_static_register_flags()  {
  static unordered_map<string, __flag_type__> __flags__;
  return __flags__;
}

RegisterFlag::RegisterFlag(const string& name,
                           __data_type_pointer__ ptr,
                           const char* descrip) {
  get_static_register_flags().insert(make_pair(name, __flag_type__(ptr, descrip)));
}

static void SetFlagValue(const string& key, const string& value) {
  // Find the key from registered flags.
  unordered_map<string, __flag_type__>::iterator flag =
      get_static_register_flags().find(key);
  if (flag == get_static_register_flags().end())  {
    LOG(FATAL) << "Error: Unknown flag \"" << key << "\".";
  }
  // Check of double-set.
  if (flag->second.setByUser) {
    LOG(FATAL) << "Flag " << key << " is set more than once.";
  } else {
    flag->second.setByUser = true;
  }
  // Assign the value to the flag.
  void* flag_pointer = flag->second.dataTypePointer.pointer;
  switch (flag->second.dataTypePointer.type) {
    case BoolType:
      *reinterpret_cast<bool*>(flag_pointer) =
          StringConvert::ToBoolAndCheck(value);
      break;
    case CharType:
      if (value.size() != 1)  {
        LOG(FATAL) << "Error: Cannot convert the flag value \"" << value
                   << "\" to char.";
      } else  {
        *reinterpret_cast<char*>(flag_pointer) = value[0];
      }
      break;
    case IntType:
      *reinterpret_cast<int*>(flag_pointer) =
          StringConvert::ToIntAndCheck(value);
      break;
    case LongType:
      *reinterpret_cast<long*>(flag_pointer) =
          StringConvert::ToLongAndCheck(value);
      break;
    case DoubleType:
      *reinterpret_cast<double*>(flag_pointer) =
          StringConvert::ToDoubleAndCheck(value);
      break;
    case StringType:
      *reinterpret_cast<string*>(flag_pointer) = value;
      break;
    default:
      LOG(FATAL) << "Internal error: Unknown flat type!";
  }
}

// Check whether the string contains whitespace.
static bool HasWhitespace(const string& str) {
  for (size_t i = 0; i < str.size(); ++i) {
    if (isspace(str[i])) {
      return true;
    }
  }
  return false;
}

// Use a text file to set flags.
static void SetFlagsFromFile(const char* filename) {
  FILE* fp = fopen(filename, "r");
  if (fp == NULL) {
    LOG(FATAL) << "Could not open flag file " << filename;
  }
  string line = FileIO::ReadLineToString(fp);
  while (line.size() != 0) {
    StringUtils::StripComments(&line, StringUtils::ShellComment);
    StringUtils::StripWhiteSpace(&line);
    if (line.size() > 0) {
      if (line.size() < 3 || line[0] != '-' || line[1] != '-' ) {
        LOG(FATAL) << "Error: Unable to process option \"" << line[0] << "\": "
                   << "all options should start with two dashes (--).";
      }
      size_t equal_pos = line.find('=');
      string key = line.substr(2, equal_pos-2);
      string value = "";
      if (equal_pos != string::npos) {
        value = line.substr(equal_pos + 1);
      } else {
        // Single "--opt" is considered as "--opt=True", except for special flags.
        value = "True";
      }
      // Check white space.
      if (HasWhitespace(key) || HasWhitespace(value)) {
        LOG(FATAL) << "Error: Unable to parse line \n"
                   << line << std::endl
                   << "One flag per line and no whitespace in key or value";
      }
      SetFlagValue(key, value);
    }
    line = FileIO::ReadLineToString(fp);
  }
  fclose(fp);
}

void CommandLineFlagsInit(int* argc, char*** argv, bool remove_flag) {
  int num_options = 0;
  for (int i = 1; i < *argc; ++i) {
    // Remove flag.
    if (remove_flag)  {
      (*argv)[i-num_options] = (*argv)[i];
    }
    // Check whether current argv is an option (starts with double dashes --).
    char* argv_i = (*argv)[i];
    CHECK_GT(strlen(argv_i), 0);
    if (argv_i[0] != '-') {
      continue;
    }
    if (strlen(argv_i) < 3 || argv_i[1] != '-') {
      LOG(FATAL) << "Error: Unable to process option \"" << argv_i << "\": "
                 << "all options should start with two dashes (--).";
    }
    ++num_options;
    // Split the option into key-value pair by equal sign (=).
    string opt(argv_i + 2);
    DCHECK(!opt.empty());
    size_t equal_pos = opt.find('=');
    string key = opt.substr(0, equal_pos);
    string value = "";
    if (equal_pos != string::npos)  {
      value = opt.substr(equal_pos + 1);
    } else  {
      // Single "--opt" is considered as "--opt=True", except for special flags.
      if (key == "flagfile") {
        LOG(FATAL) << "Error: flagfile not specified.";
      }
      value = "True";
    }
    if (key == "flagfile") {
      // Read flag from a file.
      SetFlagsFromFile(value.c_str());
    } else {
      // Set the current flag value.
      SetFlagValue(key, value);
    }
  }
  if (remove_flag)  {
    (*argc) -= num_options;
  }
}

void PrintCommandLineFlagsHelp() {
  const int leftLen = 20;
  string leftSpace = "                    ";
  for (int iFlag = 0; iFlag < __flag_type__::nFlags; ++iFlag) {
    unordered_map<string, __flag_type__>::iterator flag;
    for (flag = get_static_register_flags().begin();
         flag != get_static_register_flags().end();
         ++flag) {
      if (flag->second.id != iFlag) continue;
      // Print 'flag:' part.
      LOG(PLAIN) << flag->first << ":";
      int flagLen = flag->first.size();
      if (flagLen+1 < leftLen) {
        for (int i = 0; i < leftLen-flagLen-1; ++i) LOG(PLAIN) << " ";
      } else {
        LOG(PLAIN) << "\n" << leftSpace;
      }
      // Print 'description' part.
      const char* s = flag->second.descrip.c_str();
      while (*s) {
        LOG(PLAIN) << *s;
        if (*s == '\n') LOG(PLAIN) << leftSpace;
        ++s;
      }
      LOG(PLAIN) << "\n";
      // Print '[Type: . Default: .]' part.
      LOG(PLAIN) << leftSpace;
      void* flag_pointer = flag->second.dataTypePointer.pointer;    
      switch (flag->second.dataTypePointer.type) {
        case BoolType:
          LOG(PLAIN) << "[Type: bool. Default: "
                     << *reinterpret_cast<bool*>(flag_pointer) << ".]\n";
          break;
        case CharType:
          LOG(PLAIN) << "[Type: char. Default: '"
                     << *reinterpret_cast<char*>(flag_pointer) << "'.]\n";
          break;
        case IntType:
          LOG(PLAIN) << "[Type: int. Default: "
                     << *reinterpret_cast<int*>(flag_pointer) << ".]\n";
          break;
        case LongType:
          LOG(PLAIN) << "[Type: long. Default: "
                     << *reinterpret_cast<long*>(flag_pointer) << ".]\n";
          break;
        case DoubleType:
          LOG(PLAIN) << "[Type: double. Default: "
                     << *reinterpret_cast<double*>(flag_pointer) << ".]\n";
          break;
        case StringType:
          LOG(PLAIN) << "[Type: string. Default: \""
                     << *reinterpret_cast<string*>(flag_pointer) << "\".]\n";
          break;
        default:
          LOG(FATAL) << "Internal error: Unknown flat type!";
      }
    }
  }
}
}   // namespace __CommandLineFlags__
}   // namespace xyUtils
