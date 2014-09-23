/**
  * Utilities for checking numerical calculation.
  *
  * Author: Ying Xiong.
  * Created: Apr 16, 2014.
  */

#ifndef __XYUTILS_NUMERICAL_CHECK_H__
#define __XYUTILS_NUMERICAL_CHECK_H__

#include <Eigen/Core>

#include "NumericalFunctionTypes.h"

namespace xyUtils  {

// Checkwhether the two vectors/matrices 'm1' and 'm2' are close to each other
// under tolerance 'tol', in the sense that
//   (absolute)   || m1 - m2 || <= tol,   ** or **
//   (relative)   || m1 - m2 || / max( ||m1||, ||m2||, eps ) <= tol,
// where ||.|| is the Frobenius norm.
bool CheckNear(Eigen::MatrixXd m1, Eigen::MatrixXd m2, double tol,
               bool exit_on_fail = true);

// Same as 'CheckNear' but only check in the "absolute" sense.
bool CheckNearAbs(Eigen::MatrixXd m1, Eigen::MatrixXd m2, double tol,
                  bool exit_on_fail = true);

// Same as 'CheckNear' but only check in the "relative" sense.
bool CheckNearRel(Eigen::MatrixXd m1, Eigen::MatrixXd m2, double tol,
                  bool exit_on_fail = true);

// Numerically check the Jacobian matrix calculation of 'fcnJac'. The Jacobian
// matrix is correct if the following vectors are close
//   * f(x) - f(x0)
//   * J(x0) \cdot (x - x0)
// We consider them to be close if **either one** of the following is true
//   1. "aboslutely" close with tolerance m*||x-x0|| (see "CheckNearAbs");
//   2. "relatively" close with tolerance M*||x-x0|| (see "CheckNearRel").
bool CheckJacobian(const VectorFunctionJacobian& fcnJac,
                   void* params,
                   int N,
                   bool exit_on_fail = true,
                   double delta = 1e-4,
                   double m = 0.01,
                   double M = 10.0,
                   const Eigen::VectorXd* x0 = NULL,
                   const Eigen::VectorXd* dx = NULL);
}   // namespace xyUtils

#endif   // __XYUTILS_NUMERICAL_CHECK_H__
