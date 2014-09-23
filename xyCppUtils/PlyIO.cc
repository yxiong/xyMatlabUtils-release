/**
  * Implementation for PlyIO class.
  *
  * Author: Ying Xiong.
  * Created: Jun 16, 2013.
  */

#include "PlyIO.h"

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#include "FileIO.h"
#include "LogAndCheck.h"
#include "StringConvert.h"
#include "StringUtils.h"

using namespace std;
using namespace xyUtils;

namespace xyUtils  {
void PlyIO::ReadFile(const char* filename) {
  FILE* fp = fopen(filename, "r");
  CHECK(fp);
  // Read magic number of format.
  string line = FileIO::ReadLineToString(fp);
  CHECK(line == "ply\n");
  line = FileIO::ReadLineToString(fp);
  CHECK(line == "format ascii 1.0\n");
  // Read and process until 'end_header'.
  line = FileIO::ReadLineToString(fp);
  while (!line.empty() && (line != "end_header\n")) {
    if (strncmp(line.c_str(), "comment ", 8) == 0) continue;
    vector<string> parts = StringUtils::Split(line.c_str());
    if (parts[0] == "element") {
      CHECK(parts.size() == 3);
      elements_.push_back(Element(parts[1].c_str(),
                                 StringConvert::ToIntAndCheck(parts[2])));
    } else if (parts[0] == "property") {
      CHECK(elements_.size() > 0);
      CHECK(parts.size() == 3);
      elements_.back().AddProperty(parts[2].c_str(), parts[1].c_str());
    } else {
      LOG(ERROR) << "Unknown tag \"" << parts[0] << "\"";
    }
    line = FileIO::ReadLineToString(fp);
  }
  for (size_t i = 0; i < elements_.size(); ++i) {
    elements_[i].ReadPropertiesFromFile(fp);
  }
  fclose(fp);
}

int PlyIO::GetElementNum(const char* elemName) {
  int id = GetElementIdByName(elemName);
  CHECK(id >= 0);
  return elements_[id].size;
}

void PlyIO::ClearData() {
  for (size_t i = 0; i < elements_.size(); ++i) {
    elements_[i].ClearData();
  }
}

PlyIO::PropertyType PlyIO::PropertyTypeFromStr(const char* type) {
  if (strcmp(type, "uchar") == 0) {
    return t_uchar;
  } else if (strcmp(type, "float") == 0) {
    return t_float;
  } else {
    LOG(FATAL) << "Unknown property type \"" << type << "\"";
    return t_unknown;
  }
}

int PlyIO::GetElementIdByName(const char* elemName) const {
  for (size_t i = 0; i < elements_.size(); ++i) {
    if (elements_[i].name == elemName)      return i;
  }
  return -1;
}

void PlyIO::Element::AddProperty(const char* name, const char* type) {
  propertyNames.push_back(name);
  propertyTypes.push_back(PropertyTypeFromStr(type));
  propertyLists.push_back(PropertyList());
}

void PlyIO::Element::ReadPropertiesFromFile(FILE* fp) {
  // Resize all the property list.
  for (size_t i = 0; i < propertyTypes.size(); ++i) {
    switch (propertyTypes[i]) {
      case t_uchar:
        propertyLists[i].v_uchar.resize(size);
        break;
      case t_float:
        propertyLists[i].v_float.resize(size);
        break;
      default:
        LOG(FATAL) << "Internal error: should never get here!";
    }
  }
  // Read property data.
  for (int i = 0; i < size; ++i) {
    for (size_t j = 0; j < propertyLists.size(); ++j) {
      switch (propertyTypes[j]) {
        case t_uchar: {
          int nRead = fscanf(fp, "%hhu", &propertyLists[j].v_uchar[i]);
	  CHECK_EQ(nRead, 1);
          break;
	}
        case t_float: {
          int nRead = fscanf(fp, "%f", &propertyLists[j].v_float[i]);
	  CHECK_EQ(nRead, 1);
          break;
	}
        default:
          LOG(FATAL) << "Internal error: should never get here!";
      }
    }
  }
}

int PlyIO::Element::GetPropertyIdByName(const char* propName) const {
  for (size_t i = 0; i < propertyNames.size(); ++i) {
    if (propertyNames[i] == propName)      return i;
  }
  return -1;
}

void PlyIO::Element::ClearData() {
  for (size_t i = 0; i < propertyLists.size(); ++i) {
    switch (propertyTypes[i]) {
      case t_uchar:
        propertyLists[i].v_uchar.clear();
        break;
      case t_float:
        propertyLists[i].v_float.clear();
        break;
      default:
        LOG(FATAL) << "Internal error: should never get here!";
    }
  }
}
}   // namespace xyUtils
