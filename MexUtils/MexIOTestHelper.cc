/**
 * Usage:
 *   [d2, v2, m2, md2, str2] = MexIOTestHelper(d, v, m, md, str);
 *
 * This is a helper mex file that performs test on 'MexIO'. It will read in a
 * double 'd', a vector 'v', a matrix 'm', a 3-dimensional array 'md', and a
 * string 'str', and add 42 to all numerical types and revert 'str', and write
 * the result to output.
 *
 * Author: Ying Xiong.
 * Created: Sep 23, 2014.
 */

#include <string>
#include <mex.h>

#include "MexIO.h"

using namespace std;

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
  if (nrhs != 5)   { mexErrMsgTxt("Must have 5 input arguments.\n"); }

  // Double.
  double d = readDouble(prhs[0]);
  double d2 = d + 42;
  writeDouble(d2, &plhs[0]);

  // Vector.
  const double* v;
  int len;
  readVector(prhs[1], &v, &len);
  vector<double> v2(len);
  for (int i = 0; i < len; ++i) {
    v2[i] = v[i] + 42;
  }
  writeMatrix(v2.data(), len, 1, &plhs[1]);

  // Matrix.
  const double* m;
  int M, N;
  readMatrix(prhs[2], &m, &M, &N);
  vector<double> m2(M*N);
  for (int i = 0; i < M; ++i) {
    for (int j = 0; j < N; ++j) {
      m2[i + j*M] = m[i + j*M] + 42;
    }
  }
  writeMatrix(m2.data(), M, N, &plhs[2]);

  // Multi-dimensional array.
  const double* md;
  int dims[3];
  readMultidimArray(prhs[3], 3, &md, dims);
  vector<double> md2(dims[0] * dims[1] * dims[2]);
  for (int i = 0; i < dims[0]; ++i) {
    for (int j = 0; j < dims[1]; ++j) {
      for (int k = 0; k < dims[2]; ++k) {
        int idx = i + dims[0] * (j + dims[1] * k);
        md2[idx] = md[idx] + 42;
      }
    }
  }
  writeMultidimArray(md2.data(), 3, dims, &plhs[3]);
  
  // String
  string str = readString(prhs[4]);
  string str2;
  for (int i = str.length()-1; i >= 0; --i) {
    str2 += str[i];
  }
  writeString(str2.data(), &plhs[4]);
}
