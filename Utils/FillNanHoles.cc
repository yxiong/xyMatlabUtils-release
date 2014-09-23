/*
 * Author: Ying Xiong.
 * Created: Mar 29, 2014.
 */

// System headers.
#include <set>
#include <vector>
#include "mex.h"
// Third party headers.
#include "Eigen/Dense"
// xyCppUtils headers.
#include "LogAndCheck.h"
// mexUtils headers.
#include "mexIO.h"

using namespace std;
using namespace Eigen;

struct Vector2iComp {
  bool operator()(const Vector2i& v1, const Vector2i& v2) {
    return v1(0)<v2(0) || (v1(0)==v2(0) && v1(1)<v2(1));
  }
};

// A 'seed' is a 'NaN' pixel whose filling value can be determined in the
// current / next round.
struct Seed {
  Vector2i px;
  double val;
  // Constructor with only location.
  Seed (int i, int j) : px(i,j), val(0) { }
};

// Given a potential seed 's' with its location set, check whether that can be
// filled or not. If it can be filled, the 'val' field of 's' will be filled.
bool IsSeed(const double* D, int M, int N,
            const vector<Vector2i>& fillDirecs, const vector<double> fillScales,
            Seed* s) {
  double val = 0, n = 0;
  for (int i = 0; i < fillDirecs.size(); ++i) {
    Vector2i px = s->px + fillDirecs[i];
    if (px(0) < 0 || px(0) >= M || px(1) < 0 || px(1) >= N)   continue;
    double d = D[px(0) + px(1)*M];
    if (mxIsNaN(d))   continue;
    val += d * fillScales[i];
    n += fillScales[i];
  }
  if (n > 0) {
    s->val = val / n;
    return true;
  } else {
    return false;
  }
}

void mexFunction(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  // Read input and set default.
  CHECK(nrhs <= 3);
  // Input matrix 'D0'.
  int M, N;
  const double* D0;
  readMatrix(prhs[0], &D0, &M, &N);
  // Filling directions.
  vector<Vector2i> fillDirecs;
  if (nrhs > 1) {
    int tmpM, tmpN;
    const double* fd;
    readMatrix(prhs[1], &fd, &tmpM, &tmpN);
    CHECK_EQ(tmpM, 2);
    for (int i = 0; i < tmpN; ++i) {
      fillDirecs.push_back(Vector2i(fd[2*i], fd[2*i+1]));
    }
  } else {
    fillDirecs.push_back(Vector2i(1,0));
    fillDirecs.push_back(Vector2i(-1,0));
    fillDirecs.push_back(Vector2i(0,1));
    fillDirecs.push_back(Vector2i(0,-1));
  }
  // Filling scales.
  vector<double> fillScales;
  if (nrhs > 2) {
    const double* fs;
    readVector(prhs[2], &fs, fillDirecs.size());
    for (int i = 0; i < fillDirecs.size(); ++i) {
      fillScales.push_back(fs[i]);
    }
  } else {
    fillScales.resize(fillDirecs.size(), 1.0);
  }

  // Initialization.
  vector<Seed> seeds;     // Pixels that can be filled in the next round.
  set<Vector2i, Vector2iComp> nanPx;     // Pixels that cannot be filled in the
                                         // next round.
  vector<double> D(M*N);   // Output.
  for (int i = 0; i < M; ++i) {
    for (int j = 0; j < N; ++j) {
      D[i+j*M] = D0[i+j*M];
      if (mxIsNaN(D0[i+j*M])) {
        Seed s(i,j);
        if (IsSeed(D0, M, N, fillDirecs, fillScales, &s)) {
          seeds.push_back(s);
        } else {
          nanPx.insert(Vector2i(i,j));
        }
      }
    }
  }

  // Do the job: fill all the current seeds, and create new seeds from nanPx.
  while (!seeds.empty()) {
    vector<Seed> curSeeds;
    curSeeds.swap(seeds);
    for (int i = 0; i < curSeeds.size(); ++i) {
      // Put current seed into 'D'.
      Seed s = curSeeds[i];
      D[s.px(0) + s.px(1)*M] = s.val;
      // Add currend seed's neighbors to the next iteration of seeds.
      for (int j = 0; j < fillDirecs.size(); ++j) {
        Vector2i px = s.px - fillDirecs[j];
        int isNan = nanPx.erase(px);
        if (isNan)   seeds.push_back(Seed(px(0), px(1)));
      }
    }
    // Fill the seed value for next iteration.
    for (int i = 0; i < seeds.size(); ++i) {
      IsSeed(D.data(), M, N, fillDirecs, fillScales, &seeds[i]);
    }
  }

  // Write output.
  writeMatrix(D.data(), M, N, &plhs[0]);
}
