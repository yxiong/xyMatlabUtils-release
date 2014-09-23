/**
  * Numerical function types. This header file defines a number of numerical
  * function types as short-hand notations. All the function type names should
  * be considered as "private" and are subject to future change.
  *
  * Author: Ying Xiong.
  * Created: Apr 16, 2014.
  */

#ifndef __XYUTILS_NUMERICAL_FUNCTION_TYPES_H__
#define __XYUTILS_NUMERICAL_FUNCTION_TYPES_H__

namespace xyUtils  {

// Compute the function and its Jacobian. Note that the output 'J' matrix could
// be NULL, in which case the Jacobian is not needed by the user.
typedef void (*VectorFunctionJacobian)(const Eigen::VectorXd& x,
                                       const void* params,
                                       Eigen::VectorXd* f,
                                       Eigen::MatrixXd* J);

}   // namespace xyUtils

#endif   // __XYUTILS_NUMERICAL_FUNCTION_TYPES_H__
