/**
  * Utilities for checking numerical calculation.
  *
  * Author: Ying Xiong.
  * Created: Apr 16, 2014.
  */

#include "NumericalCheck.h"

#include <cstdlib>
#include <limits>
#include <Eigen/Core>

#include "EigenUtils.h"
#include "LogAndCheck.h"
#include "NumericalFunctionTypes.h"

namespace xyUtils  {

bool CheckNear(Eigen::MatrixXd m1, Eigen::MatrixXd m2, double tol,
               bool exit_on_fail) {
  Eigen::MatrixXd diff = m1 - m2;
  double errAbs = diff.norm();
  double errRel = errAbs / std::max(std::max(m1.norm(), m2.norm()),
                                    std::numeric_limits<double>::epsilon());
  if ((errAbs<=tol) || (errRel<=tol)) {
    return true;
  } else {
    if (exit_on_fail) {
      LOG(FATAL) << "CheckNear failed:\n"
                 << "||m1-m2|| = " << errAbs << "\n"
                 << "||m1-m2|| / max(||m1||, ||m2||, eps) = " << errRel << "\n"
                 << "Tolerance = " << tol;
    }
    return false;
  }
}

bool CheckNearAbs(Eigen::MatrixXd m1, Eigen::MatrixXd m2, double tol,
                  bool exit_on_fail) {
  Eigen::MatrixXd diff = m1 - m2;
  double errAbs = diff.norm();
  if (errAbs <= tol) {
    return true;
  } else {
    if (exit_on_fail) {
      LOG(FATAL) << "CheckNearAbs failed:\n"
                 << "||m1-m2|| = " << errAbs << "\n"
                 << "Tolerance = " << tol;
    }
    return false;
  }
}

bool CheckNearRel(Eigen::MatrixXd m1, Eigen::MatrixXd m2, double tol,
                  bool exit_on_fail) {
  Eigen::MatrixXd diff = m1 - m2;
  double errRel = diff.norm() / std::max(std::max(m1.norm(), m2.norm()),
                                         std::numeric_limits<double>::epsilon());
  if (errRel <= tol) {
    return true;
  } else {
    if (exit_on_fail) {
      LOG(FATAL) << "CheckNearRel failed:\n"
                 << "||m1-m2|| / max(||m1||, ||m2||, eps) = " << errRel << "\n"
                 << "Tolerance = " << tol;
    }
    return false;
  }
}

bool CheckJacobian(const VectorFunctionJacobian& fcnJac, void* params, int N,
                   bool exit_on_fail, double delta, double m, double M,
                   const Eigen::VectorXd* x0, const Eigen::VectorXd* dx) {
  // Set default 'x0' and 'dx'.
  Eigen::VectorXd default_x0, default_dx;
  if (!x0) {
    default_x0 = EigenUtils::RandnVectorXd(N, rand());
    x0 = &default_x0;
  }
  if (!dx) {
    default_dx = EigenUtils::RandnVectorXd(N, rand());
    default_dx /= default_dx.norm();
    dx = &default_dx;
  }
  // Evaluate the function.
  Eigen::VectorXd x = (*x0) + delta * (*dx);
  Eigen::VectorXd f0, f;
  Eigen::MatrixXd J0;
  fcnJac(*x0, params, &f0, &J0);
  fcnJac(x, params, &f, NULL);
  // Do the check.
  Eigen::VectorXd v1 = f - f0;
  Eigen::VectorXd v2 = J0 * (x - (*x0));
  double errAbs = (v1-v2).norm();
  double errRel = errAbs / std::max(std::max(v1.norm(), v2.norm()),
                                    std::numeric_limits<double>::epsilon());
  if ((errAbs <= m*delta) || (errRel <= M*delta)) {
    return true;
  } else {
    if (exit_on_fail) {
      LOG(FATAL) << "CheckJacobian failed:\n"
                 << "Absolute difference = " << errAbs
                 << " > m * ||x-x0|| = " << m << " * " << delta
                 << " = " << m*delta << "\n"
                 << "Relative difference = " << errRel
                 << " > M * ||x-x0|| = " << M << " * " << delta
                 << " = " << M*delta;
    }
    return false;
  }
}

}   // namespace xyUtils
