/**
 * Mex I/O utilities.
 *
 * Author: Ying Xiong.
 * Modified: Dec 14, 2012,
 *           Sep 22, 2014.
 */

#include <vector>
#include <string>

#include "mex.h"
#include "MexIO.h"

using namespace std;

double readDouble(const mxArray *xData) {
    // Check for double class
    if (mxGetClassID(xData)!= mxDOUBLE_CLASS) {
        mexErrMsgTxt("readMatrix: input must be double format!\n");
        return 0;
    }
    return *mxGetPr(xData);
}

void writeDouble(double xIn, mxArray **xOut) {
    *xOut = mxCreateDoubleMatrix(1, 1, mxREAL);
    *mxGetPr(*xOut) = xIn;
}

void readMatrix(const mxArray *xData, const double** xValues,
                int* nRows, int* nCols) {
  // Check for double class
  if (mxGetClassID(xData)!= mxDOUBLE_CLASS) {
    mexErrMsgTxt("readMatrix: input must be double format!\n");
    return;
  }
  *xValues = mxGetPr(xData);
  *nRows = mxGetM(xData);
  *nCols = mxGetN(xData);
}

void readMatrix(const mxArray *xData, const double** xValues,
                int nRows, int nCols) {
  int _nRows, _nCols;
  readMatrix(xData, xValues, &_nRows, &_nCols);
  if ((_nRows != nRows) || (_nCols != nCols)) {
    mexErrMsgTxt("readMatrix: the specified size is not the same as "
                 "the actual matrix size!\n");
  }
}

void readVector(const mxArray* xData, const double** xValues, int* len) {
  // Check for double class
  if (mxGetClassID(xData)!= mxDOUBLE_CLASS) {
    mexErrMsgTxt("readVector: input must be double format!\n");
    return;
  }
  *xValues = mxGetPr(xData);
  int nRows = mxGetM(xData);
  int nCols = mxGetN(xData);
  if (nRows == 1) {
    *len = nCols;
  }
  else if (nCols == 1) {
    *len = nRows;
  }
  else {
    mexErrMsgTxt("readVector: input must be of size Mx1 or 1xN matrix!\n");
  }
}

void readVector(const mxArray* xData, const double** xValues, int len) {
  int _len;
  readVector(xData, xValues, &_len);
  if (len != _len) {
    mexErrMsgTxt("readVector: the specified length is not the same as "
                 "the actual vector length!\n");
  }
}

void writeBoolMatrix(const vector<bool>& xIn, int nRows, int nCols,
                     mxArray** xOut) {
  *xOut = mxCreateLogicalMatrix(nRows, nCols);
  mxLogical* x = (mxLogical*)mxGetData(*xOut);
  int nCells = nRows * nCols;
  for (int i = 0; i < nCells; ++i)    x[i] = mxLogical(xIn[i]);
}

void readString(const mxArray *xData, char** xString, int* xLength) {
  if (mxGetClassID(xData)!= mxCHAR_CLASS) {
      mexErrMsgTxt("readString: input must be char format!\n");
  }
  int len = mxGetN(xData)+1;
  if (xLength) {
    *xLength = len;
  }
  *xString = (char*)mxCalloc(len, sizeof(char));
  mxGetString(xData, *xString, len);
}

void writeString(const char* xIn, mxArray** xOut) {
  *xOut = mxCreateString(xIn);
}

std::string readString(const mxArray *xData) {
  if (mxGetClassID(xData) != mxCHAR_CLASS) {
    mexErrMsgTxt("readString: input must be char format!\n");
  }
  int len = mxGetN(xData)+1;
  char* tmp = (char*)mxCalloc(len, sizeof(char));
  mxGetString(xData, tmp, len);
  std::string str(tmp);
  mxFree(tmp);
  return str;
}

void readMultidimArray(const mxArray *xData, int nDims,
                       const double** xValues, int* dims) {
  if (mxGetClassID(xData)!= mxDOUBLE_CLASS) {
    mexErrMsgTxt("readMultidimArray: input must be double format!\n");
    return;
  }
  if (int(mxGetNumberOfDimensions(xData)) != nDims) {
    mexErrMsgTxt("readMultidimArray: number of dimensions wrong!\n");
    return;
  }
  *xValues = mxGetPr(xData);
  const mwSize* mDims = mxGetDimensions(xData);
  for (int i = 0; i < nDims; ++i) {
    dims[i] = mDims[i];
  }
}
