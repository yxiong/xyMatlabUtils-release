/**
 * Mex I/O utilities.
 *
 * Author: Ying Xiong.
 * Modified: Dec 14, 2012,
 *           Sep 22, 2014.
 */

#ifndef __MEX_IO_H__
#define __MEX_IO_H__

#include <vector>
#include <string>

#include "mex.h"

/* Read a double variable from matlab.*/
double readDouble(const mxArray *xData);
/* Write a double variable to matlab.*/
void writeDouble(double xIn, mxArray **xOut);

/* Read a double matrix from MATLAB.
 * The output array is indexed in the following way:
 *   xData(i, j) = xValues[(i-1) + (j-1)*nRows]
 * After calling the function, 'xValues' will point to the matlab physical
 * memory, meaning that:
 *   1. If the data inside 'xData' is changed, the matlab data will also be
 *      changed when the mex file exit.
 *   2. Users do not need to free the space for 'xValues'.
 *   3. The complexity of this function is O(1).
 */
void readMatrix(const mxArray *xData, const double** xValues,
                int* nRows, int* nCols);
/* Same as above, except that the 'nRows' and 'nCols' are inputs and checked
 * against 'xData'.*/
void readMatrix(const mxArray *xData, const double** xValues,
                int nRows, int nCols);

/* Read a double vector from MATLAB.
 * Same as 'readMatrix', except that we check whether one of the dimension is 1,
 * and return the other dimension as 'len'. The input 'xData' can either be a
 * row vector or column vector.
 */
void readVector(const mxArray *xData, const double** xValues, int* len);
/* Same as above, except that the 'len' is input and checked against 'xData'.*/
void readVector(const mxArray *xData, const double** xValues, int len);

/* Write a C matrix into matlab. The C matrix is represented as an array. Any C
 * type 'T' convertible to double is supported, and the output will always be a
 * matlab 'double' matrix. The output matrix is indexed in the following way:
 *   xOut(i, j) = xIn[(i-1) + (j-1)*nRows].
 * The complexity of this function is O(nRows x nCols).
 */
template <class T>
void writeMatrix(const T* xIn, int nRows, int nCols, mxArray** xOut);

/* Same as 'writeMatrix', except the output is a boolean matrix instead of
 * double. Note that in STL, std::vector<bool> has a special implementation to
 * save space, and std::vector<bool>::data() does not work. That is why we
 * provide a function that takes std::vector<bool> directly.
 */
void writeBoolMatrix(const std::vector<bool>& xIn, int nRows, int nCols,
                     mxArray** xOut);

/* Same as 'writeMatrix', except that the index system is changed to:
 *   xOut(i, j) = xIn[(j-1) + (i-1)*nCols].
 */
template <class T>
void writeMatrixTranspose(const T* xIn, int nRows, int nCols, mxArray** xOut);

/* Read a matlab string into C. This function will allocate the memory and
 * put the content into 'xString'. The string length is saved in 'xLength'.
 * The 'xLength' pointer can be set to NULL if the user does not want the
 * length information.*/
void readString(const mxArray *xData, char** xString, int* xLength);
/* Read a matlab string and return an STL string in C. This function is
 * preferred over the previous one as the user does not need to worry about
 * de-allocating the memory.*/
std::string readString(const mxArray* xData);

/** Write a C string into matlab.*/
void writeString(const char* xIn, mxArray** xOut);

/* Read a matlab multi-dimensional array into C.
 * The output array is indexed in the following way:
 *   xData(i, j, k, ...) = xValues[(i-1) + (j-1)*n1 + (k-1)*n1*n2 + ...]
 * After calling the function, 'xValues' will point to the matlab physical
 * memory, meaning that:
 *   1. If the data inside 'xData' is changed, the matlab data will also be
 *      changed when the mex file exit.
 *   2. Users do not need to free the space for 'xValues'.
 *   3. The complexity of this function is O(1).
 * Note that the user need to preallocate the memory for 'dims' array.
 */
void readMultidimArray(const mxArray *xData, int nDims,
                       const double** xValues, int* dims);

/* Write a C array into matlab as a multi-dimensional array. Any C type 'T'
 * convertible to double is supported, and the output will always be a matlab
 * 'double' matrix. The output matrix is indexed in the following way:
 *   xOut(i, j, k, ...) = xIn[(i-1) + (j-1)*n1 + (k-1)*n1*n2 + ...]
 * The complexity of this function is O(nElems).
 */
template <class T>
void writeMultidimArray(const T* xIn, int nDims, int* dims, mxArray** xOut);

/*****************************************************************
 * Template function implementations.
 *****************************************************************/
template <class T>
void writeMatrix(const T* xIn, int nRows, int nCols, mxArray** xOut) {
  *xOut = mxCreateDoubleMatrix(nRows, nCols, mxREAL);
  double* x = mxGetPr(*xOut);
  int nCells = nRows * nCols;

  int i;
  for (i=0; i<nCells; i++)
  {
    x[i] = static_cast<double>(xIn[i]);
  }
}

template <class T>
void writeMatrixTranspose(const T* xIn, int nRows, int nCols, mxArray** xOut) {
  *xOut = mxCreateDoubleMatrix(nRows, nCols, mxREAL);
  double* x = mxGetPr(*xOut);

  for (int i = 0; i < nRows; ++i) {
    for (int j = 0; j < nCols; ++j) {
      x[i+j*nRows] = static_cast<double>(xIn[j+i*nCols]);
    }
  }
}

template <class T>
void writeMultidimArray(const T* xIn, int nDims, int* dims, mxArray** xOut) {
  // Note: since the 'mxCreateNumericArray' function take 'mwSize*' as input
  // for 'dims', we need to do the following conversion.
  mwSize* dims_mw = new mwSize[nDims];
  for (int i = 0; i < nDims; ++i) {
    dims_mw[i] = dims[i];
  }
  *xOut = mxCreateNumericArray(nDims, dims_mw, mxDOUBLE_CLASS, mxREAL);
  double* x = mxGetPr(*xOut);
  int nElem = 1;
  for (int i = 0; i < nDims; ++i) {
    nElem *= dims[i];
  }
  for (int i = 0; i < nElem; ++i) {
    x[i] = static_cast<double>(xIn[i]);
  }
  delete[] dims_mw;
}

#endif
