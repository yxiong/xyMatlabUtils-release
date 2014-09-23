/**
  * Data type utilities.
  *
  * Author: Ying Xiong
  * Created: Aug 15, 2012
  */

#ifndef __XYUTILS_DATA_TYPES_H__
#define __XYUTILS_DATA_TYPES_H__

namespace xyUtils  {
enum __data_type__  {
  InvalidType, BoolType, CharType, IntType, LongType, DoubleType, StringType
};

struct __data_type_pointer__  {
  __data_type_pointer__()
      : type(InvalidType), pointer(NULL)  {
  }

  __data_type_pointer__(__data_type__ _type, void* _pointer)
      : type(_type), pointer(_pointer)  {
  }

  __data_type__ type;
  void* pointer;
};
}   // namespace xyUtils

#endif    // __XYUTILS_DATA_TYPES_H__
