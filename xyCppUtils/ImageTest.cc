/**
  * Test for Image class.
  *
  * Author: Ying Xiong.
  * Created: May 13, 2013.
  */

#include "Image.h"

#include <string>

#ifdef __USE_TR1__
#include <tr1/cstdint>
#include <tr1/type_traits>
#else
#include <cstdint>
#include <type_traits>
#endif

#include "LogAndCheck.h"
#include "Timer.h"

using namespace std;
using namespace xyUtils;

#ifdef __USE_TR1__
using namespace std::tr1;
#endif

template <typename T>
void JpegImageTestHelper(Image<T>& image) {
  string imgName = "TestData/Images/libjpeg-testorig.jpg";
  image.LoadMetaFromFile(imgName);
  CHECK_EQ(image.GetWidth(), 227);
  CHECK_EQ(image.GetHeight(), 149);
  CHECK_EQ(image.GetNumChannels(), 3);

  image.LoadFromFile(imgName);
  CHECK_EQ(image.GetWidth(), 227);
  CHECK_EQ(image.GetHeight(), 149);
  CHECK_EQ(image.GetNumChannels(), 3);
  CHECK_EQ(image.Pixel(12, 34, 2), (PixelValueConvert<T, uint8_t>(54)));
  image.Pixel(12, 34, 1) = 25;
  CHECK_EQ(image.Pixel(12, 34, 1), 25);
  T* clr = image.PixelPr(200, 100);
  // Note: this is different from the matlab readout [98, 120, 73].
  // My understanding that different decompression algorithm can have different
  // results, which is yet confirmed.
  CHECK_EQ(clr[0], (PixelValueConvert<T, uint8_t>(97)));
  CHECK_EQ(clr[1], (PixelValueConvert<T, uint8_t>(121)));
  CHECK_EQ(clr[2], (PixelValueConvert<T, uint8_t>(71)));
}

template <typename T>
void PngImageTestHelper(Image<T>& image) {
  vector<string> imgNames;
  imgNames.push_back("TestData/Images/pngsuite/basn0g01.png");
  imgNames.push_back("TestData/Images/pngsuite/basn0g02.png");
  imgNames.push_back("TestData/Images/pngsuite/basn0g04.png");
  imgNames.push_back("TestData/Images/pngsuite/basn0g08.png");
  imgNames.push_back("TestData/Images/pngsuite/basn0g16.png");
  imgNames.push_back("TestData/Images/pngsuite/basn2c08.png");
  imgNames.push_back("TestData/Images/pngsuite/basn2c16.png");
  // Image size and number of channels.
  int imgWidth = 32, imgHeight = 32;
  int numChannels[] = {1, 1, 1, 1, 1, 3, 3};
  // Pixel value at particular positions.
  int x1 = 11, y1 = 14, c1 = 0;
  uint16_t pixelVals1[] = {65535, 85*257, 85*257, 51*257, 32512, 255*257, 42281};
  int x2 = 23, y2 = 21, c2 = 2;   // c2 will be 0 for single channel image.
  uint16_t pixelVals2[] = {0, 170*257, 170*257, 185*257, 63744, 255*257, 27482};
  // Do the check.
  for (size_t i = 0; i < imgNames.size(); ++i) {
    // Check image size by metadata.
    image.LoadMetaFromFile(imgNames[i]);
    CHECK_EQ(image.GetWidth(), imgWidth);
    CHECK_EQ(image.GetHeight(), imgHeight);
    CHECK_EQ(image.GetNumChannels(), numChannels[i]);
    // Check reading the image contents.
    image.LoadFromFile(imgNames[i]);
    CHECK_EQ(image.Pixel(x1, y1, c1),
             (PixelValueConvert<T, uint16_t>(pixelVals1[i])));
    CHECK_EQ(int(image.Pixel(x2, y2, std::min(c2, image.GetNumChannels()-1))),
             int(PixelValueConvert<T, uint16_t>(pixelVals2[i])));
  }
}

template <typename T>
void PpmImageTestHelper(Image<T>& image) {
  string imgName = "TestData/Images/libjpeg-testimg.ppm";
  image.Clear();
  CHECK_EQ(image.GetWidth(), -1);
  image.LoadMetaFromFile(imgName);
  CHECK_EQ(image.GetWidth(), 227);
  CHECK_EQ(image.GetHeight(), 149);
  CHECK_EQ(image.GetNumChannels(), 3);

  image.LoadFromFile(imgName);
  CHECK_EQ(image.GetWidth(), 227);
  CHECK_EQ(image.GetHeight(), 149);
  CHECK_EQ(image.GetNumChannels(), 3);
  CHECK_EQ(image.Pixel(12, 34, 2), (PixelValueConvert<T, uint8_t>(54)));
  CHECK_EQ(image.Pixel(200, 100, 1), (PixelValueConvert<T, uint8_t>(121)));
}

template <typename T>
void BilinearInterpTestHelper(Image<T>& image) {
  int width = 10, height = 25;
  image.SetSize(width, height, 3);
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      for (int c = 0; c < 3; ++c) {
        image.Pixel(x,y,c) = c*100 + x + y;
      }
    }
  }
  double x = 5.3, y = 6.4;
  CHECK_NEAR(image.BilinearInterp(x, y, 1), 111.7, 1e-4);
}

// A general helper to test on variouse type of 'Image<T>' class.
template <typename T>
void ImageTestHelper_General(Image<T>& image) {
  JpegImageTestHelper(image);
  PngImageTestHelper(image);
  PpmImageTestHelper(image);
  // Test for float type only.
  if ((is_same<T, float>::value || is_same<T, double>::value)) {
    BilinearInterpTestHelper(image);
  }
}

int main()  {
  Timer timer;
  LOG(INFO) << "Test on Image class...";

  Image_8u image_8u;
  ImageTestHelper_General(image_8u);
  Image_16u image_16u;
  ImageTestHelper_General(image_16u);
  Image_32f image_32f;
  ImageTestHelper_General(image_32f);
  Image_64f image_64f;
  ImageTestHelper_General(image_64f);
  
  LOG(INFO) << "Passed. [" << timer.elapsed() << " seconds]";
  return 0;
}
