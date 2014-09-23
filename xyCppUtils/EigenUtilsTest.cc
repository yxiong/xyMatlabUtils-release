/**
 * Test for Eigen library utilities.
 *
 * Author: Ying Xiong.
 * Created: Apr 21, 2014.
 */

#include "EigenUtils.h"

#include <string>

#include "LogAndCheck.h"
#include "NumericalCheck.h"
#include "Timer.h"

using namespace std;
using namespace Eigen;
using namespace xyUtils;
using namespace xyUtils::EigenUtils;

int main() {
  Timer timer;
  LOG(INFO) << "Test on EigenUtils ...";

  VectorXd v1(4);
  v1 << 1.0, 2.0, 3.0, 4.0;
  VectorXd v2 = VectorXdFromTextFile("TestData/Texts/EigenUtilsTest_RowVector.txt");
  CheckNear(v1, v2, 1e-6);
  v2 = VectorXdFromTextFile("TestData/Texts/EigenUtilsTest_ColVector.txt");
  CheckNear(v1, v2, 1e-6);

  MatrixXd m1(2, 3);
  m1 << 1.0, 2.0, 3.0, 4.0, 5.0, 6.0;
  MatrixXd m2 = MatrixXdFromTextFile("TestData/Texts/EigenUtilsTest_Matrix.txt");
  CheckNear(m1, m2, 1e-6);

  LOG(INFO) << "Passed.";
  return 0;
}
