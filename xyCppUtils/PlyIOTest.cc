/**
  * Test for PlyIO class.
  *
  * Author: Ying Xiong.
  * Created: Jun 16, 2013.
  */

#include "PlyIO.h"

#include "LogAndCheck.h"
#include "Timer.h"

using namespace xyUtils;

int main()  {
  Timer timer;
  LOG(INFO) << "Test on PlyIO ...";

  PlyIO ply;
  ply.ReadFile("TestData/Models/dinoSparseRing-pmvs.ply");
  CHECK_EQ(ply.GetElementNum("vertex"), 23928);

  std::vector<double> nx(23928);
  ply.FillArrayByProperty("vertex", "nx", nx.data());
  CHECK_NEAR(nx[0], 0.865834, 1e-6);
  CHECK_NEAR(nx[5], 0.815572, 1e-6);
  
  std::vector<unsigned char> colors(23928 * 3);
  ply.FillArrayByProperty("vertex", "diffuse_green", &colors[1], 3);
  CHECK_EQ(colors[3*1 + 1], 56);
  CHECK_EQ(colors[3*23926 + 1], 104);

  ply.ClearData();
  CHECK_EQ(ply.GetElementNum("vertex"), 23928); // Header still available.

  LOG(INFO) << "Passed. [" << timer.elapsed() << " seconds]";
  return 0;
}
