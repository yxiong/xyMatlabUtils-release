/**
  * Test on nonlinear least squares optimization.
  *
  * Author: Ying Xiong.
  * Created: Apr 16, 2014.
  */

#include "NonlinearLeastSquares.h"

#include <cmath>
#include <cstdlib>

#ifdef __USE_TR1__
#include <tr1/random>
#else
#include <random>
#endif

#include "EigenUtils.h"
#include "LogAndCheck.h"
#include "NumericalCheck.h"
#include "Timer.h"

using namespace std;
using namespace Eigen;
using namespace xyUtils;

#ifdef __USE_TR1__
using namespace std::tr1;
#endif

struct TestFcnParams {
  VectorXd t;
  VectorXd m;

  // Constructor.
  TestFcnParams(int M) : t(M), m(M) { }
};

/**
 * A test function that fits the model
 *   model(x; t) = x[2] * exp(x[0]*t) + x[3] * exp(x[1]*t),
 * and the cost function to be minimized is
 *   fcn(x) = model(x; t) - m.
 */
static void TestFcn(const VectorXd& x, const void* params,
                    VectorXd* f, MatrixXd* J) {
  const TestFcnParams* fcnParams = static_cast<const TestFcnParams*>(params);
  const VectorXd& t = fcnParams->t;
  const VectorXd& m = fcnParams->m;
  VectorXd exp_x0t = (x(0)*t).array().exp();
  VectorXd exp_x1t = (x(1)*t).array().exp();
  *f = x(2) * exp_x0t + x(3) * exp_x1t - m;
  if (J) {
    J->resize(f->size(), x.size());
    J->col(0) = x(2) * t.cwiseProduct(exp_x0t);
    J->col(1) = x(3) * t.cwiseProduct(exp_x1t);
    J->col(2) = exp_x0t;
    J->col(3) = exp_x1t;
  }
}

int main()  {
  Timer timer;
  LOG(INFO) << "Test on NonlinearLeastSquares ...";

#ifdef __USE_TR1__
  ranlux64_base_01 generator;
#else
  default_random_engine generator;
#endif
  normal_distribution<double> randn(5.0, 2.0);

  // Ground truth model parameters.
  int N = 4;
  VectorXd x_gt(N);
  x_gt << 1.0, -0.5, 2.0, 3.0;
  VectorXd x_gt2(N);
  x_gt2 << -0.5, 1.0, 3.0, 2.0;

  // Generate data.
  int M = 10;
  TestFcnParams params(M);
  params.t = EigenUtils::RandnVectorXd(M, rand());
  params.m = x_gt(2) * (x_gt(0)*params.t).array().exp() +
      x_gt(3) * (x_gt(1)*params.t).array().exp();
  CheckJacobian(TestFcn, &params, N);

  // Run nonlinear optimization.
  VectorXd x0 = VectorXd::Zero(N);
  NLLSOpts nllsOpts;
  NLLSResultInfo nllsResult;
  VectorXd x = NonlinearLeastSquares(TestFcn, &params, x0, nllsOpts, &nllsResult);

  // Check against ground truth.
  CHECK(CheckNear(x, x_gt, 0.01, false) ||
        CheckNear(x, x_gt2, 0.01, false));
  // Check whether the result is correctly calculated.
  VectorXd f;
  TestFcn(x, &params, &f, NULL);
  double F = f.squaredNorm();
  CHECK_NEAR(F, nllsResult.F, 1e-6);

  // Bounded optimization.
  nllsOpts.upperBound = VectorXd(4);
  nllsOpts.upperBound(0) = 0.5;
  nllsOpts.upperBound(1) = nan("");
  nllsOpts.upperBound(2) = nan("");
  nllsOpts.upperBound(3) = nan("");

  x = NonlinearLeastSquares(TestFcn, &params, x0, nllsOpts, NULL);
  CheckNear(x, x_gt2, 0.01);

  LOG(INFO) << "Passed. [" << timer.elapsed() << " seconds]";
  return 0;
}
