/**
  * PointCameraViewer class, a visualizer for 3D point cloud and cameras
  * observing them, implemented in SDL.
  *
  * Example usage:
  *   PointEdgeViewer viewer;
  *   viewer.Launch();
  *   viewer.AddPoints(...);
  *   viewer.AddEdges(...);    // Optional.
  *   viewer.SetImgSize(width, height);
  *   viewer.AddCameras(...);
  *   viewer.CenterView();
  *
  * Keyboard and mouse interactions: same as PointEdgeViewer, plus the follwoings
  *   'd', 'f': decrease/increase camera sizes.
  *   'g': toggle between showing and hiding cameras.
  *
  * Author: Ying Xiong.
  * Created: Jun 16, 2013.
  */

#ifndef __XYUTILS_POINTS_CAMERA_VIEWER_H__
#define __XYUTILS_POINTS_CAMERA_VIEWER_H__

#include <vector>

#include "Camera.h"
#include "PointEdgeViewer.h"

namespace xyUtils  {
class PointCameraViewer : public PointEdgeViewer {
 public:
  // Constructor.
  PointCameraViewer() : camSize_(1), showCameras_(true) { }
  // Destructor. Wait until the event thread exists.
  virtual ~PointCameraViewer() {
    if (event_thread_)    SDL_WaitThread(event_thread_, NULL);
  }
  // Erase everything in the viewer.
  virtual void Clear();
  // Set image size. Note that this needs to be done before adding cameras.
  void SetImgSize(double width, double height) {
    imgWidth_ = width; imgHeight_ = height;
  }
  // Add 'cameras' for visualization. 
  void AddCameras(const std::vector<Camera>& cameras);
  
 protected:
  // Main event handler. Return true if the 'event' is handled.
  virtual bool HandleEvent(SDL_Event event);
  // Render to the screen.
  virtual void Render();

  // Cameras to be visualized.
  std::vector<Camera> cameras_;
  // The camera size for visualization.
  double camSize_;
  // Image size information is necessary for visualizing cameras.
  double imgWidth_;
  double imgHeight_;
  // Whether to show cameras or not.
  bool showCameras_;
};
}   // namespace xyUtils

#endif   // __XYUTILS_POINTS_CAMERA_VIEWER_H__
