/**
  * PlyIO class, for read/write *.ply model files.
  *
  * Example usage:
  *   PlyIO ply;
  *   ply.ReadFile("/Path/to/file.ply");
  *   std::vector<vector> xyz(ply.GetElementNum("vertex") * 3);
  *   ply.FillArrayByProperty("vertex", "x", &xyz[0], 3);
  *   ply.FillArrayByProperty("vertex", "y", &xyz[1], 3);
  *   ply.FillArrayByProperty("vertex", "z", &xyz[2], 3);
  *   // Read other data.
  *   ply.Clear(); // or "ply.ClearData();" if want to keep header information.
  *
  * Author: Ying Xiong.
  * Created: Jun 16, 2013.
  */

#ifndef __XYUTILS_PLY_IO_H__
#define __XYUTILS_PLY_IO_H__

#include <cstdio>
#include <string>
#include <vector>

#include "LogAndCheck.h"

namespace xyUtils  {
class PlyIO {
 public:
  // ================================================================
  // Public interface.
  // ================================================================
  
  // Read data from a .ply file.
  void ReadFile(const char* filename);
  // Get number of 'elemName' elements.
  int GetElementNum(const char* elemName);
  // Fill the 'array' by property 'propName' of element 'elemName'. The optional
  // 'stride' specifies the steps taken when filling the vector, e.g. fill every
  // other element if 'stride' is 2. Note that the user needs to pre-allocate
  // memory for 'array'.
  template<typename T>
  void FillArrayByProperty(const char* elemName, const char* propName,
                            T* array, int stride = 1) const;
  // Clear everything in the object.
  void Clear() { elements_.clear(); }
  // Clear the substantial data and therefore release memory. The header
  // information (e.g. number of vertices) will still be available.
  void ClearData();
  
 private:
  // ================================================================
  // Nested structures.
  // ================================================================
  
  // A 'PropertyList' is a collection (vector) of scalars or lists. Note that
  // only one of the field should be non-empty. In other words, it is like a
  // union of vectors, but C++ does not allow union of STL containers.
  struct PropertyList {
    std::vector<unsigned char> v_uchar;
    std::vector<float> v_float;
  };
  // A 'PropertyType' is an enum specifying the property type.
  enum PropertyType {
    t_unknown, t_uchar, t_float
  };
  // An 'Element' is a collection (vector) of 'PropertyList' with corresponding
  // 'PropertyType' and property names. Technically, an 'Element' is a list of
  // elements. This is the top-level structure of 'PlyIO', which contains a
  // vector of 'Element'.
  struct Element {
    // Create an Element with no actual data.
    Element(const char* _name, int _size) : name(_name), size(_size) { }
    // Add a property to the list.
    void AddProperty(const char* name, const char* type);
    // Read property data from the file 'fp'.
    void ReadPropertiesFromFile(FILE* fp);
    // Get property index by name, return -1 if no such property exists.
    int GetPropertyIdByName(const char* propName) const;
    // Clear the substantial data and therefore release memory. The header
    // information (e.g. property types and names) will still be available.
    void ClearData();
    // Data fields.
    std::string name; // Element name, e.g. 'vertex'.
    int size;  // Number of this element in the ply file, should be the same as
               // propertyXxxx[y].size() if the data are filled.
    std::vector<std::string> propertyNames;
    std::vector<PropertyType> propertyTypes;
    std::vector<PropertyList> propertyLists;
  };

  // ================================================================
  // Private functions.
  // ================================================================

  // Get the PropertyType from a type name string.
  static PropertyType PropertyTypeFromStr(const char* type);
  // Get element index by name, return -1 if no such element exists.
  int GetElementIdByName(const char* elemName) const;

  // ================================================================
  // Private members.
  // ================================================================
  std::vector<Element> elements_;
};

// ================================================================
// Implementation for templated functions.
// ================================================================
template<typename T>
void PlyIO::FillArrayByProperty(const char* elemName, const char* propName,
                                 T* array, int stride) const {
  // Get the right element and property.
  int elemId = GetElementIdByName(elemName);
  CHECK(elemId >= 0);
  const Element& element = elements_[elemId];
  int propId = element.GetPropertyIdByName(propName);
  CHECK(propId >= 0);
  const PropertyType& pType = element.propertyTypes[propId];
  const PropertyList& pList = element.propertyLists[propId];
  // Read data according to property type.
  switch (pType) {
    case t_uchar:
      for (int i = 0; i < element.size; ++i) array[i*stride] = pList.v_uchar[i];
      break;
    case t_float:
      for (int i = 0; i < element.size; ++i) array[i*stride] = pList.v_float[i];
      break;
    default:
      LOG(FATAL) << "Internal error: should never get here!";
  }
}
}   // namespace xyUtils

#endif   // __XYUTILS_PLY_IO_H__
