/**
  * Nonlinear least squares optimization.
  *
  * Author: Ying Xiong.
  * Created: Apr 16, 2014.
  */

#ifndef __XYUTILS_NONLINEAR_LEAST_SQUARES_H__
#define __XYUTILS_NONLINEAR_LEAST_SQUARES_H__

#include <Eigen/Core>

#include "NumericalFunctionTypes.h"

namespace xyUtils  {

struct NLLSOpts {
  enum AlgorithmType { ALGORITHM_LM };
  enum DisplayType { DISPLAY_OFF, DISPLAY_FINAL, DISPLAY_FINAL_DETAILED,
                     DISPLAY_ITER, DISPLAY_ITER_DETAILED };
  // Options for Levenberg-Marquardt algorithm.
  struct LMOpts {
    enum DampMatrix { DAMP_MATRIX_EYE, DAMP_MATRIX_JJ };
    // Constructor that sets default values.
    LMOpts() :
        dampMatrix(DAMP_MATRIX_EYE), tau(1.0e-3) { }

    DampMatrix dampMatrix;
    double tau;
  };

  // Constructor that sets default values.
  NLLSOpts() :
      algorithm(ALGORITHM_LM), display(DISPLAY_OFF), lmOpts(),
      upperBound(), lowerBound(),
      tolX(1e-6), tolF(1e-6), maxIter(400) { }

  AlgorithmType algorithm;
  DisplayType display;
  LMOpts lmOpts;
  Eigen::VectorXd upperBound;
  Eigen::VectorXd lowerBound;
  double tolX;
  double tolF;
  int maxIter;
};

struct NLLSResultInfo {
  int exitflag;
  int finalIter;
  double F;
};

Eigen::VectorXd NonlinearLeastSquares(
    const VectorFunctionJacobian& fcnJac,
    void* params,
    const Eigen::VectorXd& x0,
    const NLLSOpts& opts,
    NLLSResultInfo* result);


}   // namespace xyUtils

#endif   // __XYUTILS_NONLINEAR_LEAST_SQUARES_H__
