/**
  * Utility functions for quaternion.
  *
  * In this file, a quaternion ('Quat') is described by a 4-vector q:
  *     q[0] + q[1]*i + q[2]*j + q[3]*k.
  *
  * A rotation matrix ('Matx') is described by a 9-vector R:
  *     | R[0]  R[1]  R[2] |
  *     | R[3]  R[4]  R[5] |
  *     | R[6]  R[7]  R[8] |.
  * Note that a ratation matrix should be orthogonal with determinant 1:
  *     R'*R = I, det(R) = 1.
  *
  * A 3D vector ('Vect') is described by a 3-vector v:
  *     (v[0], v[1], v[2])'.
  *
  * Author: Ying Xiong.
  * Created: May 24, 2013.
  */

#ifndef __XYUTILS_QUATERNION_H__
#define __XYUTILS_QUATERNION_H__

#include <cmath>

#include "LogAndCheck.h"

namespace xyUtils  {
namespace Quaternion {

// Return +1 if x>0, -1 if x<0, and 0 if x==0.
inline double sign(double x) {
  if (x>0)      return 1.0;
  else if (x<0) return -1.0;
  return 0.0;
}

// Print a vector.
inline void PrintVect(const double* v) {
  LOG(PLAIN) << "(" << v[0] << ", " << v[1] << ", " << v[2] << ")";
}

// Print a quaternion.
inline void PrintQuat(const double* q) {
  LOG(PLAIN) << "(" << q[0] << ", " << q[1] << ", "
             << q[2] << ", " << q[3] << ")";
}

// Print a rotation matrix.
inline void PrintMatx(const double* M) {
  LOG(PLAIN) << M[0] << " " << M[1] << " " << M[2] << "\n"
             << M[3] << " " << M[4] << " " << M[5] << "\n"
             << M[6] << " " << M[7] << " " << M[8] << "\n";

}

// Compute the dot product of vector 'a' and 'b'.
inline double VectDot(const double* a, const double* b) {
  return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

// Compute the 2-norm of vector 'v'.
inline double VectNorm(const double* v) {
  return sqrt(VectDot(v, v));
}

// Compute the cross product of vector 'a' and 'b', and put the result in 'c'.
inline void VectCross(const double* a, const double* b, double* c) {
  c[0] = a[1]*b[2] - a[2]*b[1];
  c[1] = a[2]*b[0] - a[0]*b[2];
  c[2] = a[0]*b[1] - a[1]*b[0];
}

// Compute the angle between vector a and vector b.
inline double VectAngle(const double* a, const double* b) {
  return acos(VectDot(a, b) / VectNorm(a) / VectNorm(b));
}

// Normalize vector 'v' so that it will have unit length.
inline void NormalizeVect(double* v) {
  double n = VectNorm(v);
  v[0]/=n; v[1]/=n; v[2]/=n;
}

// Compute the norm of the quaternion 'q':
//     q[0]^2 + q[1]^2 + q[2]^2 + q[3]^2.
inline double QuatNorm(const double* q) {
  return sqrt(q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3]);
}

// Normalize quaternion 'q' so that it will have unit norm.
inline void NormalizeQuat(double* q) {
  double n = QuatNorm(q);
  q[0]/=n; q[1]/=n; q[2]/=n; q[3]/=n;
}

// Compute the conjugate of quaternion 'q':
//     qc = q[0] - q[1]*i - q[2]*j - q[3]*k.
inline void QuatConjugate(const double* q, double* qc) {
  qc[0]=q[0]; qc[1]=-q[1]; qc[2]=-q[2]; qc[3]=-q[3];
}

// Compute the reciprocal of the quaternion 'q', such that:
//     q * qr = 1
inline void QuatReciprocal(const double* q, double* qr) {
  QuatConjugate(q, qr);
  double n2 = q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3];
  qr[0]/=n2; qr[1]/=n2; qr[2]/=n2; qr[3]/=n2;
}

// Compute the hamilton product of two quaterion 'a' and 'b', and put the result
// in 'c'.
inline void QuatHamiltonProduct(const double* a, const double* b, double* c) {
  c[0] = a[0]*b[0] - a[1]*b[1] - a[2]*b[2] - a[3]*b[3];
  c[1] = a[0]*b[1] + a[1]*b[0] + a[2]*b[3] - a[3]*b[2];
  c[2] = a[0]*b[2] - a[1]*b[3] + a[2]*b[0] + a[3]*b[1];
  c[3] = a[0]*b[3] + a[1]*b[2] - a[2]*b[1] + a[3]*b[0];
}

// Get quaternion 'q' representing a rotation of angle 'theta' around the axis
// defined by a **unit vector** 'u':
//     q = cos(theta/2) + sin(theta/2) * (ux*i + uy*j + uz*k).
inline void QuatFromVectAngle(const double* u, double theta, double* q) {
  q[0] = cos(theta/2);
  double s = sin(theta/2);
  q[1] = s * u[0];
  q[2] = s * u[1];
  q[3] = s * u[2];
}

// Convert a quaternion 'q' to a rotation matrix 'R'. The input quaternion
// should represent a rotation (have unit norm).
inline void QuatToMatx(const double* q, double* R) {
  R[0] = 1 - 2*(q[2]*q[2] + q[3]*q[3]);
  R[4] = 1 - 2*(q[1]*q[1] + q[3]*q[3]);
  R[8] = 1 - 2*(q[1]*q[1] + q[2]*q[2]);
  
  R[1] = 2*q[1]*q[2] - 2*q[0]*q[3];
  R[3] = 2*q[1]*q[2] + 2*q[0]*q[3];
  
  R[2] = 2*q[1]*q[3] + 2*q[0]*q[2];
  R[6] = 2*q[1]*q[3] - 2*q[0]*q[2];
  
  R[5] = 2*q[2]*q[3] - 2*q[0]*q[1];
  R[7] = 2*q[2]*q[3] + 2*q[0]*q[1];
}

// Convert a rotation matrix 'R' to a quaternion 'q'.
inline void MatxToQuat(const double* R, double* q) {
  q[0] = ( R[0] + R[4] + R[8] + 1.0f) / 4.0f;
  q[1] = ( R[0] - R[4] - R[8] + 1.0f) / 4.0f;
  q[2] = (-R[0] + R[4] - R[8] + 1.0f) / 4.0f;
  q[3] = (-R[0] - R[4] + R[8] + 1.0f) / 4.0f;
  if(q[0] < 0.0f) q[0] = 0.0f;
  if(q[1] < 0.0f) q[1] = 0.0f;
  if(q[2] < 0.0f) q[2] = 0.0f;
  if(q[3] < 0.0f) q[3] = 0.0f;
  q[0] = sqrt(q[0]);
  q[1] = sqrt(q[1]);
  q[2] = sqrt(q[2]);
  q[3] = sqrt(q[3]);
  if(q[0] >= q[1] && q[0] >= q[2] && q[0] >= q[3]) {
    q[0] *= +1.0f;
    q[1] *= sign(R[7] - R[5]);
    q[2] *= sign(R[2] - R[6]);
    q[3] *= sign(R[3] - R[1]);
  } else if(q[1] >= q[0] && q[1] >= q[2] && q[1] >= q[3]) {
    q[0] *= sign(R[7] - R[5]);
    q[1] *= +1.0f;
    q[2] *= sign(R[3] + R[1]);
    q[3] *= sign(R[2] + R[6]);
  } else if(q[2] >= q[0] && q[2] >= q[1] && q[2] >= q[3]) {
    q[0] *= sign(R[2] - R[6]);
    q[1] *= sign(R[3] + R[1]);
    q[2] *= +1.0f;
    q[3] *= sign(R[7] + R[5]);
  } else if(q[3] >= q[0] && q[3] >= q[1] && q[3] >= q[2]) {
    q[0] *= sign(R[3] - R[1]);
    q[1] *= sign(R[6] + R[2]);
    q[2] *= sign(R[7] + R[5]);
    q[3] *= +1.0f;
  } else {
    LOG(FATAL) << "Internal error!";
  }
  double n = QuatNorm(q);
  q[0] /= n;
  q[1] /= n;
  q[2] /= n;
  q[3] /= n;
}

// Convert a unit quaternion 'q' to a 3-vector 'v'. This is also the effect of
// taking logarithm of 'q'.
//     ln ( cos(theta) + sin(theta) * (a i + b j + c k) )
//   = theta * (a i + b j + c k)
inline void QuatToVect(const double* q, double* v) {
  double theta = acos(q[0]);
  if (theta > 0) {
    double s = sin(theta);
    v[0] = theta*q[1]/s;
    v[1] = theta*q[2]/s;
    v[2] = theta*q[3]/s;
  } else {
    v[0]=0; v[1]=0; v[2]=0;
  }
}

// Convert a 3-vector 'v' to a unit quaternion, the inverse of 'QuatToVect'.
inline void VectToQuat(const double* v, double* q) {
  double theta = VectNorm(v);
  if (theta > 0) {
    q[0] = cos(theta);
    double s = sin(theta);
    q[1] = v[0]*s/theta;
    q[2] = v[1]*s/theta;
    q[3] = v[2]*s/theta;
  } else {
    q[0]=1; q[1]=0; q[2]=0; q[3]=0;
  }
}

// Rotate vector 's' by a quaternion 'q' and put the result in 't'. The input
// 's' and 't' can be the same pointer.
inline void RotateVectByQuat(const double* s, const double* q, double* t) {
  double p[4] = {0, s[0], s[1], s[2]};
  double qc[4];
  QuatConjugate(q, qc);
  double p2[4], tmp[4];
  QuatHamiltonProduct(q, p, tmp);
  QuatHamiltonProduct(tmp, qc, p2);
  t[0]=p2[1]; t[1]=p2[2]; t[2]=p2[3];
}

// Rotate vector 's' across axis 'a' for angle 'ang' (in unit of radian), and
// result in 't'. The axis 'a' needs to have unit length. The input 's' and 't'
// can be the same pointer.
inline void RotateVectByAxisAngle(const double* s, const double* a, double ang,
                                  double* t) {
  double q[4];
  QuatFromVectAngle(a, ang, q);
  RotateVectByQuat(s, q, t);
}

}   // namespace Quaternion
}   // namespace xyUtils

#endif   // __XYUTILS_QUATERNION_H__
