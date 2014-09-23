/**
  * Nonlinear least squares optimization.
  *
  * Author: Ying Xiong.
  * Created: Apr 16, 2014.
  */

#include "NonlinearLeastSquares.h"

#include <iomanip>
#include <limits>
#include <Eigen/Core>
#include <Eigen/Cholesky>

#include "LogAndCheck.h"

namespace xyUtils  {

namespace {
void PrintIterInfoHeader_LM(const NLLSOpts& opts) {
  if (opts.display >= NLLSOpts::DISPLAY_ITER) {
    LOG(PLAIN) << std::setw(5) << "Iters" << "  "
               << std::setw(15) << "F(x)";
    if (opts.display >= NLLSOpts::DISPLAY_ITER_DETAILED) {
      LOG(PLAIN) << std::setw(10) << "rho" << "  "
                 << std::setw(10) << "mu" << "  "
                 << std::setw(10) << "nu";
    }
    LOG(PLAIN) << "\n";
  }
}

void PrintIterInfo_LM(const NLLSOpts& opts, int iter, double F,
                      double rho, double mu, double nu) {
  if (opts.display >= NLLSOpts::DISPLAY_ITER) {
    LOG(PLAIN) << std::setw(5) << iter << "  "
               << std::setw(15) << std::setprecision(8) << F;
    if (opts.display >= NLLSOpts::DISPLAY_ITER_DETAILED) {
      LOG(PLAIN) << std::setw(10) << std::setprecision(3) << rho << "  "
                 << std::setw(10) << std::setprecision(3) << mu << "  "
                 << std::setw(10) << std::setprecision(3) << nu;
    }
    LOG(PLAIN) << "\n";
  }
}

void PrintFinalInfo(const NLLSOpts& opts, const NLLSResultInfo& result) {
  if (opts.display >= NLLSOpts::DISPLAY_FINAL) {
    LOG(PLAIN) << "Terminate in " << result.finalIter << " iterations: ";
    switch (result.exitflag) {
      case 0:
        LOG(PLAIN) << "maximum number of iterations reached.\n";
        break;
      case 1:
        LOG(PLAIN) << "local minimum reached.\n";
        break;
      case 2:
        LOG(PLAIN) << "change in 'x' less than 'tolX' (" << opts.tolX << ").\n";
        break;
      case 3:
        LOG(PLAIN) << "change in 'f' less than 'tolF' (" << opts.tolF << ").\n";
        break;
      case 4:
        LOG(PLAIN) << "magnitude of search direction is less than eps.\n";
        break;
      default:
        LOG(FATAL) << "Unknown 'exitflag'.";
    }
  }
}

int StopCriterion_LM(double rho_denom, double rho,
                     double F_old, double F, double aTolF,
                     const Eigen::VectorXd& x_old, const Eigen::VectorXd& x,
                     double aTolX) {
  if (rho_denom < std::numeric_limits<double>::epsilon()) {
    // When this happens, the step size is usually very small and the
    // calculation of 'rho' is below numerical accuracy. We claim to find a
    // local minimum.
    return 1;
  }
  if (rho > 0) {
    if (F_old-F < aTolF) {
      return 3;
    } else if ((x_old-x).cwiseAbs().maxCoeff() < aTolX) {
      return 2;
    } else {
      return 0;
    }
  } else {
    return 0;
  }
}

Eigen::VectorXd NonlinearLeastSquares_LM(
    const VectorFunctionJacobian& fcnJac,
    void* params,
    const Eigen::VectorXd& x0,
    const NLLSOpts& opts,
    NLLSResultInfo* result) {
  // ================================================================
  // Initialization.
  // ================================================================
  Eigen::VectorXd x = x0;
  Eigen::VectorXd f;
  Eigen::MatrixXd J;
  fcnJac(x, params, &f, &J);
  int N = x.size();
  double F = f.squaredNorm();
  Eigen::MatrixXd Jt = J.transpose();
  Eigen::MatrixXd JJ = Jt * J;
  Eigen::VectorXd Jf = Jt * f;
  double mu = opts.lmOpts.tau * JJ.diagonal().maxCoeff();
  if (mu < std::numeric_limits<double>::epsilon()) {
    // J is a zero matrix.
    result->exitflag = 1;
    result->finalIter = 0;
    result->F = F;
    PrintFinalInfo(opts, *result);
    return x;
  }
  double mu_min = 1.0e-12;
  double nu = 2.0;
  double aTolX = opts.tolX / x.size();
  double aTolF = opts.tolF / x.size();
  // The followings might be needed for the first iterations's stop criterion,
  // in case one starts at a local minimum.
  Eigen::VectorXd x_old = x;
  double F_old = F;
  PrintIterInfoHeader_LM(opts);
  PrintIterInfo_LM(opts, 0, F, 0.0, mu, nu);
  // ================================================================
  // Main loop.
  // ================================================================
  int iter;
  for (iter = 0; iter < opts.maxIter; ++iter) {
    // Compute direction 'h'.
    Eigen::VectorXd h;
    Eigen::MatrixXd damp;
    if (opts.lmOpts.dampMatrix == NLLSOpts::LMOpts::DAMP_MATRIX_EYE) {
      damp = mu * Eigen::MatrixXd::Identity(N, N);
    } else if (opts.lmOpts.dampMatrix == NLLSOpts::LMOpts::DAMP_MATRIX_JJ) {
      damp = mu * JJ.diagonal().asDiagonal();
    } else {
      LOG(FATAL) << "Unknown 'DampMatrix' option.";
    }
    h = - (JJ + damp).llt().solve(Jf);
    // Handle boundary condition (naively).
    Eigen::VectorXd x_new = x + h;
    if (opts.upperBound.size() > 0) {
      for (int i = 0; i < N; ++i) {
        x_new(i) = std::isnan(opts.upperBound(i)) ? x_new(i) :
            std::min(x_new(i), opts.upperBound(i));
      }
    }
    if (opts.lowerBound.size() > 0) {
      for (int i = 0; i < N; ++i) {
        x_new(i) = std::isnan(opts.lowerBound(i)) ? x_new(i) :
            std::max(x_new(i), opts.lowerBound(i));
      }
    }
    // Compute the new 'f' and 'J'.
    fcnJac(x_new, params, &f, &J);
    double F_new = f.squaredNorm();
    // Compute gain ratio 'rho'.
    double rho_denom;
    if (opts.lmOpts.dampMatrix == NLLSOpts::LMOpts::DAMP_MATRIX_EYE) {
      rho_denom = h.dot(mu*h - Jf);
    } else {
      rho_denom = h.dot(damp*h - Jf);
    }
    double rho = (F - F_new) / rho_denom;
    if (rho > 0) {
      // Step accepted.
      x_old = x;   x = x_new;
      F_old = F;   F = F_new;
      Jt = J.transpose();      JJ = Jt * J;      Jf = Jt * f;
      mu = std::max(mu_min, mu*std::max(1.0/3.0, 1.0-pow(2.0*rho-1, 3)));
      nu = 2;
    } else {
      // Step not accepted.
      mu *= nu;      nu *= 2;
    }
    // Display information.
    PrintIterInfo_LM(opts, iter+1, F, rho, mu, nu);
    // Check the stop criterion.
    result->exitflag = StopCriterion_LM(rho_denom, rho, F_old, F, aTolF,
                                        x_old, x, aTolX);
    if (result->exitflag)   break;
  }
  result->finalIter = iter;
  result->F = F;
  PrintFinalInfo(opts, *result);
  return x;
}
}   // namespace

Eigen::VectorXd NonlinearLeastSquares(
    const VectorFunctionJacobian& fcnJac,
    void* params,
    const Eigen::VectorXd& x0,
    const NLLSOpts& opts,
    NLLSResultInfo* result) {
  // Create a 'ResultInfo' struct for internal use even if it is not required
  // from the user.
  NLLSResultInfo _result;
  if (!result) {
    result = &_result;
  }
  // Dispatch the job.
  switch (opts.algorithm) {
    case NLLSOpts::ALGORITHM_LM:
      return NonlinearLeastSquares_LM(fcnJac, params, x0, opts, result);
    default:
      LOG(FATAL) << "Unhandled algorithm.";
      return x0;
  }
}

}   // namespace xyUtils
