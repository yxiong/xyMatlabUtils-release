/**
  * Some utility functions for Eigen library.
  *
  * Author: Ying Xiong.
  * Created: Apr 16, 2014.
  */

#include "EigenUtils.h"

#include <cstdio>
#include <vector>

#ifdef __USE_TR1__
#include <tr1/random>
#else
#include<random>
#endif

#include "FileIO.h"
#include "LogAndCheck.h"
#include "StringConvert.h"
#include "StringUtils.h"

namespace xyUtils  {
namespace EigenUtils  {
Eigen::VectorXd RandnVectorXd(int N, unsigned int seed,
                              double mu, double sigma) {
#ifdef __USE_TR1__
  std::tr1::ranlux64_base_01 generator(seed);
  std::tr1::normal_distribution<double> randn(mu, sigma);
#else
  std::default_random_engine generator(seed);
  std::normal_distribution<double> randn(mu, sigma);
#endif
  Eigen::VectorXd X(N);
  for (int i = 0; i < N; ++i) {
    X(i) = randn(generator);
  }
  return X;
}

Eigen::MatrixXd RandnMatrixXd(int M, int N, unsigned int seed,
                              double mu, double sigma) {
#ifdef __USE_TR1__
  std::tr1::ranlux64_base_01 generator(seed);
  std::tr1::normal_distribution<double> randn(mu, sigma);
#else
  std::default_random_engine generator(seed);
  std::normal_distribution<double> randn(mu, sigma);
#endif
  Eigen::MatrixXd X(M, N);
  for (int i = 0; i < M; ++i) {
    for (int j = 0; j < N; ++j) {
      X(i,j) = randn(generator);
    }
  }
  return X;
}

Eigen::VectorXd VectorXdFromTextFile(const char* filename) {
  // In current implementation, we first read the data into a stl vector, and
  // then copy it to an Eigen::VectorXd. This should be improved once we have
  // better understanding on Eigen dynamic allocation interface.
  FILE* fp = fopen(filename, "r");
  CHECK(fp);
  std::vector<double> v_stl;
  std::string line = FileIO::ReadLineToString(fp);
  std::vector<std::string> parts = StringUtils::Split(line);
  for (size_t i = 0; i < parts.size(); ++i) {
    v_stl.push_back(StringConvert::ToDoubleAndCheck(parts[i]));
  }
  if (v_stl.size() > 1) {
    // Row vector.
    line = FileIO::ReadLineToString(fp);
    CHECK(line.empty());
  } else {
    // Column vector.
    line = FileIO::ReadLineToString(fp);
    while (!line.empty()) {
      StringUtils::StripWhiteSpace(&line);
      v_stl.push_back(StringConvert::ToDoubleAndCheck(line));
      line = FileIO::ReadLineToString(fp);
    }
  }
  fclose(fp);

  Eigen::VectorXd v(v_stl.size());
  for (size_t i = 0; i < v_stl.size(); ++i) {
    v(i) = v_stl[i];
  }
  return v;
}

Eigen::MatrixXd MatrixXdFromTextFile(const char* filename) {
  // In current implementation, we first read the data into a stl vector of stl
  // vectors, and then copy it to an Eigen::MatrixXd. This should be improved
  // once we have better understanding on Eigen dynamic allocation interface.
  FILE* fp = fopen(filename, "r");
  CHECK(fp);
  std::vector<std::vector<double> > m_stl;
  // Read the first row.
  std::string line = FileIO::ReadLineToString(fp);
  std::vector<std::string> parts = StringUtils::Split(line);
  std::vector<double> v_stl(parts.size());
  for (size_t i = 0; i < parts.size(); ++i) {
    v_stl[i] = StringConvert::ToDoubleAndCheck(parts[i]);
  }
  m_stl.push_back(v_stl);
  // Read the following rows.
  line = FileIO::ReadLineToString(fp);
  while (!line.empty()) {
    parts = StringUtils::Split(line);
    CHECK_EQ(v_stl.size(), parts.size());
    for (size_t i = 0; i < parts.size(); ++i) {
      v_stl[i] = StringConvert::ToDoubleAndCheck(parts[i]);
    }
    m_stl.push_back(v_stl);
    line = FileIO::ReadLineToString(fp);
  }
  fclose(fp);

  Eigen::MatrixXd m(m_stl.size(), v_stl.size());
  for (size_t i = 0; i < m_stl.size(); ++i) {
    for (size_t j = 0; j < v_stl.size(); ++j) {
      m(i,j) = m_stl[i][j];
    }
  }
  return m;
}
}   // namespace EigenUtils
}   // namespace xyUtils
