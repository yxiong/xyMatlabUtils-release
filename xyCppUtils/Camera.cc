/**
  * Implementation of Camera class and utility functions.
  *
  * Author: Ying Xiong.
  * Created: Jun 16, 2013.
  */

#include "Camera.h"

#include <cctype>
#include <cstdio>
#include <vector>

#include <Eigen/Dense>

#include "LogAndCheck.h"

using namespace std;
using namespace Eigen;
namespace xyUtils  {
namespace {
// Helper function to read 'n' doubles from file 'fp', and put the results in
// array 'r'. The user needs to pre-allocate memory for 'r'.
void ReadNDoublesFromFile(int n, FILE* fp, double* r) {
  for (int i = 0; i < n; ++i) {
    int nRead = fscanf(fp, "%lf", r+i);
    CHECK_EQ(nRead, 1);
  }
}
}  // namespace

Camera::Camera(const double* K, const double* R, const double* t) {
  SetKRt(K, R, t);
}

void Camera::SetKRt(const double* K, const double* R, const double* t) {
  K_ << K[0], K[1], K[2], K[3], K[4], K[5], K[6], K[7], K[8];
  R_ << R[0], R[1], R[2], R[3], R[4], R[5], R[6], R[7], R[8];
  t_ << t[0], t[1], t[2];
}

Vector3d Camera::center() const {
  // c = -R^{-1} t.
  return - R_.colPivHouseholderQr().solve(t_);
}

Vector3d Camera::backProjRay(double* x) const {
  // ray = - (KR)^{-1} [x;1].
  Vector3d hx(x[0], x[1], 1.0);
  Vector3d ray = (K_*R_).colPivHouseholderQr().solve(hx);
  return ray.normalized();
}

vector<Camera> ReadCamerasFromFile(const char* filename, const char* format) {
  vector<Camera> cameras;
  FILE* fp = fopen(filename, "r");
  CHECK(fp);
  if (strcmp(format, "NameKRt") == 0) {
    cameras = ReadCamerasFromFile_NameKRt(fp);
  } else if (strcmp(format, "NumNameKRt") == 0) {
    cameras = ReadCamerasFromFile_NumNameKRt(fp);
  } else {
    LOG(FATAL) << "Unknown camera file format.";
  }
  fclose(fp);
  return cameras;
}

vector<Camera> ReadCamerasFromFile_NameKRt(FILE* fp) {
  vector<Camera> cameras;
  while (1) {
    // Skip the 'name'.
    while (!feof(fp) && isspace(fgetc(fp))) ;
    while (!feof(fp) && !isspace(fgetc(fp))) ;
    if (feof(fp)) break;
    // Read actual parameters.
    double K[9];
    ReadNDoublesFromFile(9, fp, K);
    double R[9];
    ReadNDoublesFromFile(9, fp, R);
    double t[3];
    ReadNDoublesFromFile(3, fp, t);
    cameras.push_back(Camera(K, R, t));
  }
  return cameras;
}

vector<Camera> ReadCamerasFromFile_NumNameKRt(FILE* fp) {
  int nCams;
  int nRead = fscanf(fp, "%d", &nCams);
  CHECK_EQ(nRead, 1);
  vector<Camera> cameras = ReadCamerasFromFile_NameKRt(fp);
  CHECK_EQ(int(cameras.size()), nCams);
  return cameras;
}
}   // namespace xyUtils
