/**
  * Command line flag utilities.
  *
  * Example usage:
  *
  * #include "CommandLineFlags.h"
  * DEFINE_FLAG_bool(help, false, "Print this help message.");
  * // Define other flags.
  * int main(int argc, char** argv) {
  *   xyUtils::CommandLineFlagsInit(&argc, &argv, true);
  *   if (FLAGS_help) {
  *     xyUtils::PrintCommandLineFlagsHelp();
  *     return 0;
  *   }
  *   // Rest of the program.  
  * }
  *
  * Author: Ying Xiong.
  * Created: Jun 11, 2012.
  */

#ifndef __XYUTILS_COMMAND_LINE_FLAGS_H__
#define __XYUTILS_COMMAND_LINE_FLAGS_H__

#include <string>

#include "DataTypes.h"

#define DEFINE_FLAG(type_name, type, name, init_value, descrip)           \
  namespace xyUtils  {                                                    \
  namespace __CommandLineFlags__  {                                       \
    static type FLAGS_##name = init_value;                                \
    static RegisterFlag __REGISTER_##name##__(                            \
        #name, __data_type_pointer__(type_name, &FLAGS_##name), descrip); \
  }                                                                       \
  }                                                                       \
  using xyUtils::__CommandLineFlags__::FLAGS_##name;

#define DEFINE_FLAG_bool(name, init_value, descrip)                       \
  DEFINE_FLAG(BoolType, bool, name, init_value, descrip)

#define DEFINE_FLAG_char(name, init_value, descrip)                       \
  DEFINE_FLAG(CharType, char, name, init_value, descrip)

#define DEFINE_FLAG_int(name, init_value, descrip)                        \
  DEFINE_FLAG(IntType, int, name, init_value, descrip)

#define DEFINE_FLAG_long(name, init_value, descrip)                       \
  DEFINE_FLAG(LongType, long, name, init_value, descrip)

#define DEFINE_FLAG_double(name, init_value, descrip)                     \
  DEFINE_FLAG(DoubleType, double, name, init_value, descrip)

#define DEFINE_FLAG_string(name, init_value, descrip)                     \
  DEFINE_FLAG(StringType, std::string, name, init_value, descrip)

namespace xyUtils  {
namespace __CommandLineFlags__  {
class RegisterFlag  {
 public:
  RegisterFlag(const std::string& name,
               __data_type_pointer__ ptr,
               const char* descrip);
};

void CommandLineFlagsInit(int* argc, char*** argv, bool remove_flag);
void PrintCommandLineFlagsHelp();

}   // namespace __CommandLineFlags__

using __CommandLineFlags__::CommandLineFlagsInit;
using __CommandLineFlags__::PrintCommandLineFlagsHelp;

}   // namespace xyUtils

#endif  // __XYUTILS_COMMAND_LINE_FLAGS_H__
