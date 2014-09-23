/**
  * Test for quaternion utilities.
  *
  * Author: Ying Xiong.
  * Created: May 24, 2013.
  */

#include "Quaternion.h"

#include <cmath>

#include "LogAndCheck.h"
#include "Timer.h"

using namespace xyUtils;
using namespace xyUtils::Quaternion;

int main()  {
  Timer timer;
  LOG(INFO) << "Test on quaternion ...";
  double EPS = 1e-10;

  // Rotate the vector 's' to vector 't'.
  double s[3] = {sqrt(2)/2, sqrt(2)/2, sqrt(3)};
  double t[3] = {-sqrt(3)/2, -1.0/2, sqrt(3)};
  double z[3] = {0.0, 0.0, 1.0};
  double ang = 165.0 / 180.0 * M_PI;
  double t2[3];
  RotateVectByAxisAngle(s, z, ang, t2);
  CHECK_NEAR(t[0], t2[0], EPS);
  CHECK_NEAR(t[1], t2[1], EPS);
  CHECK_NEAR(t[2], t2[2], EPS);

  // Rotate across the perpendicular direction.
  double p[3];
  VectCross(s, t, p);
  NormalizeVect(p);
  CHECK_NEAR(VectDot(s, p), 0, EPS);
  CHECK_NEAR(VectDot(t, p), 0, EPS);
  ang = VectAngle(s, t);
  RotateVectByAxisAngle(s, p, ang, t2);
  CHECK_NEAR(t[0], t2[0], EPS);
  CHECK_NEAR(t[1], t2[1], EPS);
  CHECK_NEAR(t[2], t2[2], EPS);
  
  // Test quaterion, vector and rotation matrix conversion.
  double q[4] = {-1.0, 2.0, 3.0, -4.0};
  NormalizeQuat(q);
  double R[9], q2[4];
  QuatToMatx(q, R);
  MatxToQuat(R, q2);
  if (sign(q[0]) == sign(q2[0])) {
    CHECK_NEAR(q[0], q2[0], EPS);
    CHECK_NEAR(q[1], q2[1], EPS);
    CHECK_NEAR(q[2], q2[2], EPS);
    CHECK_NEAR(q[3], q2[3], EPS);
  } else {
    CHECK_NEAR(q[0], -q2[0], EPS);
    CHECK_NEAR(q[1], -q2[1], EPS);
    CHECK_NEAR(q[2], -q2[2], EPS);
    CHECK_NEAR(q[3], -q2[3], EPS);
  }

  double v[3];
  QuatToVect(q, v);
  VectToQuat(v, q2);
  CHECK_NEAR(q[0], q2[0], EPS);
  CHECK_NEAR(q[1], q2[1], EPS);
  CHECK_NEAR(q[2], q2[2], EPS);
  CHECK_NEAR(q[3], q2[3], EPS);
  
  // Test the other utilities.
  q[0]=-1.0; q[1]=2.0; q[2]=3.0; q[3]=-4.0;
  double qr[4];
  double prod[4];
  QuatReciprocal(q, qr);
  QuatHamiltonProduct(q, qr, prod);
  CHECK_NEAR(prod[0], 1.0, EPS);
  CHECK_NEAR(prod[1], 0.0, EPS);
  CHECK_NEAR(prod[2], 0.0, EPS);
  CHECK_NEAR(prod[3], 0.0, EPS);
  
  LOG(INFO) << "Passed. [" << timer.elapsed() << " seconds]";
  return 0;
}
