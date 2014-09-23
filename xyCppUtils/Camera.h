/**
  * Camera class, for a projective camera described by parameters 'K', 'R', 't'.
  *
  * Author: Ying Xiong.
  * Created: Jun 16, 2013.
  */

#ifndef __XYUTILS_CAMERA_H__
#define __XYUTILS_CAMERA_H__

#include <vector>
#include <Eigen/Core>

#include "EigenUtils.h"

namespace xyUtils  {
class Camera {
 public:
  // Constructors.
  Camera() { }
  Camera(const double* K, const double* R, const double* t);
  // Set parameters by C arrays.
  void SetKRt(const double* K, const double* R, const double* t);
  // Compute and return the camera center.
  Eigen::Vector3d center() const;
  // Compute the back projection ray of image point 'x'. The input 'x' is a
  // 2-vector, and the output is a unit vector indicating the back projection
  // direction starting from camera center.
  Eigen::Vector3d backProjRay(double* x) const;
 private:
  // Camera parameters.
  EigenUtils::rMatrix3d K_;
  EigenUtils::rMatrix3d R_;
  Eigen::Vector3d t_;
};

// Read a set of cameras from a file 'filename' of specified 'format'.
// Following 'format' are supported:
// --"NameKRt"
//   Name1 K11 K12 ... K33 R11 R12 ... R33 t1 t2 t3
//   Name2 K11 K12 ... K33 R11 R12 ... R33 t1 t2 t3
//   ......
// --"NumNameKRt"
//   NumCams
//   Name1 K11 K12 ... K33 R11 R12 ... R33 t1 t2 t3
//   Name2 K11 K12 ... K33 R11 R12 ... R33 t1 t2 t3
//   ......
std::vector<Camera> ReadCamerasFromFile(const char* filename, const char* format);
std::vector<Camera> ReadCamerasFromFile_NameKRt(FILE* fp);
std::vector<Camera> ReadCamerasFromFile_NumNameKRt(FILE* fp);
}   // namespace xyUtils

#endif   // __XYUTILS_CAMERA_H__
