/**
  * Image class.
  *
  * Author: Ying Xiong.
  * Created: May 13, 2013.
  */

#ifndef __XYUTILS_IMAGE_H__
#define __XYUTILS_IMAGE_H__

#include <cstddef>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#ifdef __USE_TR1__
#include <tr1/cstdint>
#else
#include <cstdint>
#endif

#include "jpeglib.h"
#include "png.h"

#include "LogAndCheck.h"

namespace xyUtils  {

/** Image is a templated on the image data type. For a vast majority of cases, the
 *  data type is 'unsigned char', but it can be extended to other types such as
 *  'unsigned short' (for 16-bit images) or 'float' (for HDR images).*/
template <typename T = unsigned char>
class Image {
 public:
  // Image types.
  enum ImageType {UnknownType, JpegType, PngType, PpmType};
  // ================================================================
  // Public interface.
  // ================================================================
  // Construct an invalid object.
  Image() : width_(-1), height_(-1), numChannels_(-1) { }
  // Construct an image with specified size.
  Image(int width, int height, int numChannels) :
      width_(width), height_(height), numChannels_(numChannels),
      data_(width*height*numChannels) { }
  // Get image width.
  int GetWidth() const {  return width_;  }
  // Get image height.
  int GetHeight() const { return height_;  }
  // Get number of color channels, usually either 1 or 3.
  int GetNumChannels() const {  return numChannels_; }
  // Set image set. The image data after this call are unspecified.
  void SetSize(int width, int height, int numChannels);
  // Get pixel (x, y) at channel c.
  T Pixel(int x, int y, int c = 0) const {
    return data_[Index_(x,y,c)];
  }
  T& Pixel(int x, int y, int c = 0) {
    return data_[Index_(x,y,c)];
  }
  // Get the data pointer to pixel (x, y).
  const T* PixelPr(int x, int y, int c = 0) const {
    return data_.data() + Index_(x,y,c);
  }
  T* PixelPr(int x, int y, int c = 0) {
    return data_.data() + Index_(x,y,c);
  }
  // Return the data pointer.
  const T* data() const { return data_.data(); }
  T* data() { return data_.data(); }
  // Clear the data field and reset the object to an invalid state.
  void Clear() {
    width_ = height_ = numChannels_ = -1;
    data_.clear();
  }
  // Clear the data field and reclaim most of the memory. The metadata will
  // still be valid.
  void ClearData() {
    data_.clear();
  }
  // Bilinear interpolation. Note: current implementation requires 'x' and 'y'
  // lying inside the image, and 'T' being float or double type.
  template<typename F>
  T BilinearInterp(F x, F y, int c) const;
  // Load the meta information or the whole image from an input file.
  // If the 'type' is not specified, it will be determined by the file suffix.
  void LoadMetaFromFile(const char* filename, ImageType type = UnknownType);
  void LoadMetaFromFile(const std::string& filename, ImageType type = UnknownType) {
    LoadMetaFromFile(filename.c_str(), type);
  }
  void LoadFromFile(const char* filename, ImageType type = UnknownType);
  void LoadFromFile(const std::string& filename, ImageType type = UnknownType) {
    LoadFromFile(filename.c_str(), type);
  }
  // Load the meta information or the whole image from a Jpeg file.
  // Note: currently can only read 8-bit jpeg images. (TODO)
  void LoadMetaFromJpegFile(const char* filename);
  void LoadMetaFromJpegFile(const std::string& filename) {
    LoadMetaFromJpegFile(filename.c_str());
  }
  void LoadFromJpegFile(const char* filename);
  void LoadFromJpegFile(const std::string& filename) {
    LoadFromJpegFile(filename.c_str());
  }
  // Write the image to a jpg file. The 'quality' parameter should be a number
  // between 0 and 100, with 0 being lowest quality and 100 highest. We suggest
  // use 80 for high quality output, 50 for medium and 30 for low.
  // Note: currently can only write 8-bit jpeg images. (TODO)
  void WriteToJpegFile(const char* filename, int quality) const;
  void WriteToJpegFile(const std::string& filename, int quality) const {
    WriteToJpegFile(filename.c_str(), quality);
  }
  // Load the meta information or the whole image from a png file.
  void LoadMetaFromPngFile(const char* filename);
  void LoadMetaFromPngFile(const std::string& filename) {
    LoadMetaFromPngFile(filename.c_str());
  }
  void LoadFromPngFile(const char* filename);
  void LoadFromPngFile(const std::string& filename) {
    LoadFromPngFile(filename.c_str());
  }
  // Load the meta information or the whole image from a ppm file.
  // Note: currently only works for 8-bit jpeg images. (TODO)
  void LoadMetaFromPpmFile(const char* filename);
  void LoadMetaFromPpmFile(const std::string& filename) {
    LoadMetaFromPpmFile(filename.c_str());
  }
  void LoadFromPpmFile(const char* filename);
  void LoadFromPpmFile(const std::string& filename) {
    LoadFromPpmFile(filename.c_str());
  }
  // Determine the image type from filename.
  static ImageType TypeFromFilename(const char* filename);
  static ImageType TypeFromFilename(const std::string& filename) {
    return TypeFromFilename(filename.c_str());
  }
 private:
  // ================================================================
  // Helper functions.
  // ================================================================
  int Index_(int x, int y, int c) const {
    return c + numChannels_ * (x + width_ * y);
  }
  void ReadGrayPngDataSubByte(const png_bytep* png_data, int bit_depth,
                              uint8_t mask0, int scale);
  void ReadGrayPngData(const png_bytep* png_data, int bit_depth);
  void ReadRGBPngData(const png_bytep* png_data, int bit_depth);
  void LoadMetaFromPpmFileHelper(FILE* fp);
  // ================================================================
  // Data fields.
  // ================================================================
  int width_, height_, numChannels_;
  // Store all image data in a vector. The c-th channel of (x,y)-th pixel has index
  // Index_(x, y, c).
  std::vector<T> data_;
};

typedef Image<uint8_t> Image_8u;
typedef Image<uint16_t> Image_16u;
typedef Image<float> Image_32f;
typedef Image<double> Image_64f;

// Convert pixel value from data type to another: the value range for different
// types are:
//   uint8_t:    255
//   uint16_t: 65535
//   float:      1.0
//   double:     1.0
template<typename DST_TYPE, typename SRC_TYPE>
inline DST_TYPE PixelValueConvert(SRC_TYPE val);

}   // namespace xyUtils

#include "Image.tcc"

#endif   // __XYUTILS_IMAGE_H__
