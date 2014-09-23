/**
  * Some utilities for Eigen library.
  *
  * Author: Ying Xiong.
  * Created: Jun 20, 2013.
  */

#ifndef __XYUTILS_EIGEN_UTILS_H__
#define __XYUTILS_EIGEN_UTILS_H__

#include <Eigen/Core>

namespace xyUtils  {
namespace EigenUtils {

// The following typedefs define row-majored matrices.
typedef Eigen::Matrix<double, 2, 2, Eigen::RowMajor> rMatrix2d;
typedef Eigen::Matrix<float, 2, 2, Eigen::RowMajor> rMatrix2f;
typedef Eigen::Matrix<int, 2, 2, Eigen::RowMajor> rMatrix2i;
typedef Eigen::Matrix<double, 3, 3, Eigen::RowMajor> rMatrix3d;
typedef Eigen::Matrix<float, 3, 3, Eigen::RowMajor> rMatrix3f;
typedef Eigen::Matrix<int, 3, 3, Eigen::RowMajor> rMatrix3i;
typedef Eigen::Matrix<double, 4, 4, Eigen::RowMajor> rMatrix4d;
typedef Eigen::Matrix<float, 4, 4, Eigen::RowMajor> rMatrix4f;
typedef Eigen::Matrix<int, 4, 4, Eigen::RowMajor> rMatrix4i;
typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> rMatrixXd;
typedef Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> rMatrixXf;
typedef Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> rMatrixXi;

// Generate a random vector/matrix with i.i.d. random entires following normal
// distribution with mean equal to 'mu' and standard deviation equal to 'sigma'.
// Note that the same 'seed' will result in the same matrix, and therefore
// itself is suggested to be randomized, say using rand().
Eigen::VectorXd RandnVectorXd(int N, unsigned int seed,
                              double mu = 1.0, double sigma = 1.0);
Eigen::MatrixXd RandnMatrixXd(int M, int N, unsigned int seed,
                              double mu = 1.0, double sigma = 1.0);

// Read a vector/matrix from a text file. Currently supported file format is
//   val11 val12 ...
//   val21 val22 ...
//   ......
Eigen::VectorXd VectorXdFromTextFile(const char* filename);
inline Eigen::VectorXd VectorXdFromTextFile(const std::string filename) {
  return VectorXdFromTextFile(filename.c_str());
}
Eigen::MatrixXd MatrixXdFromTextFile(const char* filename);
inline Eigen::MatrixXd MatrixXdFromTextFile(const std::string filename) {
  return MatrixXdFromTextFile(filename.c_str());
}

}   // namespace Eigen
}   // namespace xyUtils

#endif   // __XYUTILS_EIGEN_UTILS_H__
