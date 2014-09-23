/**
  * Test for numerical check.
  *
  * Author: Ying Xiong.
  * Created: Apr 16, 2014.
  */

#include "NumericalCheck.h"

#include "LogAndCheck.h"
#include "Timer.h"

using namespace Eigen;
using namespace xyUtils;

namespace {
// A test function
//   f(x) = [sin(x(0)); cos(x(1)); x(0)^2*exp(-0.5*x(1))].
void TestFcnJac(const Eigen::VectorXd& x,
                const void* params,
                Eigen::VectorXd* f,
                Eigen::MatrixXd* J) {
  (void) params;
  *f = Vector3d(sin(x(0)), cos(x(1)), x(0)*x(0)*exp(-0.5*x(1)));
  if (J) {
    *J = MatrixXd(3, 2);
    (*J)(0,0) = cos(x(0));
    (*J)(0,1) = 0;
    (*J)(1,0) = 0;
    (*J)(1,1) = -sin(x(1));
    (*J)(2,0) = 2*x(0)*exp(-0.5*x(1));
    (*J)(2,1) = -0.5*x(0)*x(0)*exp(-0.5*x(1));
  }
}

void TestFcnWrongJac(const Eigen::VectorXd& x,
                     const void* params,
                     Eigen::VectorXd* f,
                     Eigen::MatrixXd* J) {
  TestFcnJac(x, params, f, J);
  (*f) *= -1.0;
}
} // namespace

int main()  {
  Timer timer;
  LOG(INFO) << "Test on NumericalCheck ...";

  // Test 'CheckNear' on MatrixXd.
  MatrixXd m1(2, 3);
  m1 << 1.0, 2.0, 3.0, 4.0, 5.0, 6.0;
  MatrixXd m2 = m1;
  MatrixXd m3 = m1 * 2;
  CHECK(CheckNear(m1, m2, 1e-6));
  CHECK(CheckNear(m1, m3, 1e-6, false) == false);
  CHECK(CheckNearAbs(m1, m2, 1e-6));
  CHECK(CheckNearAbs(m1, m3, 1e-6, false) == false);
  CHECK(CheckNearRel(m1, m2, 1e-6));
  CHECK(CheckNearRel(m1, m3, 1e-6, false) == false);

  // Test 'CheckNear' on Vector3d.
  Vector3d v1(-1.0, -2.0, -3.0);
  VectorXd v2 = v1;
  VectorXd v3 = v1 * 2;
  CHECK(CheckNear(v1, v2, 1e-6));
  CHECK(CheckNear(v1, v3, 1e-6, false) == false);
  CHECK(CheckNearAbs(v1, v2, 1e-6));
  CHECK(CheckNearAbs(v1, v3, 1e-6, false) == false);
  CHECK(CheckNearRel(v1, v2, 1e-6));
  CHECK(CheckNearRel(v1, v3, 1e-6, false) == false);

  // Test CheckJacobian.,
  CHECK(CheckJacobian(TestFcnJac, NULL, 2));
  CHECK(CheckJacobian(TestFcnWrongJac, NULL, 2, false, 1e-4, 0.01, 1.0) == false);

  LOG(INFO) << "Passed. [" << timer.elapsed() << " seconds]";
  return 0;
}
