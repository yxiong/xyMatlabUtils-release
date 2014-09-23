/**
  * A binary file for visualizing a 3D point cloud with observing cameras.
  *
  * Author: Ying Xiong.
  * Created: Jun 18, 2013.
  */

#include <vector>

#include "Camera.h"
#include "CommandLineFlags.h"
#include "PlyIO.h"
#include "PointCameraViewer.h"

DEFINE_FLAG_bool(help, false, "Print this help message.");
DEFINE_FLAG_string(pointFile, "", "The point cloud filename.");
DEFINE_FLAG_string(pointFileType, "",
                   "Point cloud file type, currently supporing:\n"
                   "    'ply'.");
DEFINE_FLAG_string(cameraFile, "", "The camera filename.");
DEFINE_FLAG_string(cameraFileType, "",
                   "Camera file type, currently supporting:\n"
                   "    'NumNameKRt'.");

using namespace std;
using namespace xyUtils;

int main(int argc, char** argv) {
  CommandLineFlagsInit(&argc, &argv, true);
  if (FLAGS_help) {
    PrintCommandLineFlagsHelp();
    return 0;
  }
  
  // Read point file.
  CHECK_EQ(FLAGS_pointFileType, "ply");
  PlyIO ply;
  ply.ReadFile(FLAGS_pointFile.c_str());
  vector<double> points(ply.GetElementNum("vertex") * 3);
  ply.FillArrayByProperty("vertex", "x", &points[0], 3);
  ply.FillArrayByProperty("vertex", "y", &points[1], 3);
  ply.FillArrayByProperty("vertex", "z", &points[2], 3);
  vector<unsigned char> colors(ply.GetElementNum("vertex") * 3);
  ply.FillArrayByProperty("vertex", "diffuse_red", &colors[0], 3);
  ply.FillArrayByProperty("vertex", "diffuse_green", &colors[1], 3);
  ply.FillArrayByProperty("vertex", "diffuse_blue", &colors[2], 3);
  ply.Clear();

  // Read camera file (optional).
  vector<Camera> cameras;
  if (FLAGS_cameraFile != "") {
    cameras = ReadCamerasFromFile(
        FLAGS_cameraFile.c_str(), FLAGS_cameraFileType.c_str());
  }

  // Create and launch the viewer.
  PointCameraViewer viewer;
  viewer.Launch();
  viewer.AddPoints(points.size()/3, points.data(), colors.data());
  viewer.SetImgSize(640, 480);
  viewer.AddCameras(cameras);
  viewer.CenterView();

  return 0;
}
