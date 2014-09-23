/**
  * Implementation of PointCameraViewer class.
  *
  * Author: Ying Xiong.
  * Created: Jun 16, 2013.
  */

#include "PointCameraViewer.h"

#include <vector>

#include <Eigen/Core>
#include <SDL/SDL.h>

#include "Camera.h"

using namespace std;
using namespace Eigen;
namespace xyUtils  {
void PointCameraViewer::Clear() {
  PointEdgeViewer::Clear();
  cameras_.clear();
}

void PointCameraViewer::AddCameras(const vector<Camera>& cameras) {
  SDL_LockMutex(lock_);
  for (size_t i = 0; i < cameras.size(); ++i) {
    cameras_.push_back(cameras[i]);
  }
  SDL_UnlockMutex(lock_);
}

bool PointCameraViewer::HandleEvent(SDL_Event event) {
  // Let parent handle event first.
  if (PointEdgeViewer::HandleEvent(event)) {
    return true;
  }
  // Handle child's event
  SDL_LockMutex(lock_);
  bool handled = false;
  if (event.type == SDL_KEYDOWN) {
    switch (event.key.keysym.sym) {
      case SDLK_d:
        // Keyboard 'd' to decrease camera size.
        camSize_ *= 0.8;
        RequestRender();
        handled = true;
        break;
      case SDLK_f:
        // Keyboard 'f' to increase camera size.
        camSize_ *= 1.25;
        RequestRender();
        handled = true;
        break;
      case SDLK_g:
        // Keyboard 'g' to toggle between showing and hiding cameras.
        showCameras_ = !showCameras_;
        RequestRender();
        handled = true;
        break;
      default:
        // Do nothing for other keys.
        ;
    }
  }
  SDL_UnlockMutex(lock_);
  return handled;
}

void PointCameraViewer::Render() {
  SDL_LockMutex(lock_);
  // Add camera points and edges.
  if (showCameras_) {
    for (size_t i = 0; i < cameras_.size(); ++i) {
      // Camera center.
      Vector3d c = cameras_[i].center();
      for (int j = 0; j < 3; ++j)      coords_.push_back(c[j]);
      // Back projection of image corners.
      double imgCorners[8] = {0.0, 0.0,
                              imgWidth_, 0.0,
                              imgWidth_, imgHeight_,
                              0.0, imgHeight_};
      for (int j = 0; j < 4; ++j) {
        Vector3d ray = cameras_[i].backProjRay(imgCorners + j*2);
        Vector3d X = c + camSize_*ray;
        for (int k = 0; k < 3; ++k)        coords_.push_back(X[k]);
      }
      // Add new colors.
      colors_.resize(colors_.size() + 3*5, 255);
      // Add edges.
      int n = coords_.size()/3 - 5;  // Number of current points.
      int edges[16] = {n, n+1, n, n+2, n, n+3, n, n+4,
                       n+1, n+2, n+2, n+3, n+3, n+4, n+4, n+1};
      for (int j = 0; j < 16; ++j) {
        edges_.push_back(edges[j]);
      }
    }
  }
  // Draw points and edges.
  DrawPointsAndEdges();
  // Remove camera points and edges.
  if (showCameras_) {
    coords_.resize(coords_.size() - 15*cameras_.size());
    colors_.resize(colors_.size() - 15*cameras_.size());
    edges_.resize(edges_.size() - 16*cameras_.size());
  }
  render_requested_ = false;
  SDL_UnlockMutex(lock_);
}

}   // namespace xyUtils
