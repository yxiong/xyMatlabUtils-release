/**
  * PointEdgeViewer class, a visualizer for 3D point cloud and edges connecting
  * them, implemented in SDL.
  *
  * Example usage:
  *   PointEdgeViewer viewer;
  *   viewer.Launch();
  *   viewer.AddPoints(...);
  *   viewer.AddEdges(...);
  *   viewer.CenterView();
  *
  * Keyboard and mouse interactions:
  *   'c': center the view point.
  *   'Esc'/'q': exit the viewer.
  *   left-drag: rotate the scene.
  *   right-drag: translate the scene.
  *   wheel-up/down: zoom in/out.
  *
  * Author: Ying Xiong.
  * Created: Jun 13, 2013.
  */

#ifndef __XYUTILS_POINT_EDGE_VIEWER_H__
#define __XYUTILS_POINT_EDGE_VIEWER_H__

#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include "CameraByView.h"
#include "SDLViewer.h"

namespace xyUtils  {
class PointEdgeViewer : public SDLViewer {
 public:
  // Constructor.
  PointEdgeViewer();
  // Destructor. Wait until the event thread exists, then clean up.
  virtual ~PointEdgeViewer() {
    if (event_thread_)    SDL_WaitThread(event_thread_, NULL);
  }
  // Erase everything in the viewer.
  virtual void Clear();
  // Add 3D points to the scene. The input 'points' should contain 3XnPoints
  // entries. All points assume white color.
  template <typename T>
  void AddPoints(int nPoints, T* points);
  // Add 3D points with specified colors to the scene. The input 'points' and
  // 'colors' should contain 3XnPoints entries. The 'colorScale' parameter
  // specifies the scale factor to map input colors to the range [0, 255]. For
  // example, if the input color are floats from [0, 1], the 'colorScale' should
  // be 255.
  template <typename T1, typename T2>
  void AddPoints(int nPoints, T1* points, T2* colors, T2 colorScale = 1);
  // Add edges. The input 'edges' should contain 2XnEdges entries, each entry is
  // an integer specifying the point index, starting from 0.
  template <typename T>
  void AddEdges(int nEdges, T* edges);
  // Center the view by setting the new viewpoint to the average of all points.
  void CenterView();
  // Set camera distance.
  void SetCamDist(double dist);
  // Rotate the camera direction for 'ang' (radian) across up direction.
  void RotateCamAcrossUp(double ang);
  // Rotate the camera direction for 'ang' (radian) across right direction.
  void RotateCamAcrossRight(double ang);
  
 protected:
  // Initialize the SDL window.
  virtual void Init();
  // Main event handler. Return true if the 'event' is handled.
  virtual bool HandleEvent(SDL_Event event);
  // Render to the screen.
  virtual void Render();

  // Draw all points and edges.
  void DrawPointsAndEdges();

  // Data to be visualized.
  static const GLenum coord_type = GL_FLOAT;
  std::vector<GLfloat> coords_;
  static const GLenum color_type = GL_UNSIGNED_BYTE;
  std::vector<GLubyte> colors_;
  static const GLenum index_type = GL_UNSIGNED_INT;
  std::vector<GLuint> edges_;

  // Camera parameters.
  CameraByView camera_;
};

// ================================================================
// Implementation for templated functions.
// ================================================================
template <typename T>
void PointEdgeViewer::AddPoints(int nPoints, T* points) {
  SDL_LockMutex(lock_);
  int curSize = coords_.size() / 3;
  coords_.resize((curSize+nPoints)*3);
  for (int i = 0; i < 3*nPoints; ++i) {
    coords_[curSize*3+i] = points[i];
  }
  colors_.resize((curSize+nPoints)*3, 255);
  SDL_UnlockMutex(lock_);
}

template <typename T1, typename T2>
void PointEdgeViewer::AddPoints(int nPoints, T1* points,
                                 T2* colors, T2 colorScale) {
  SDL_LockMutex(lock_);
  int curSize = coords_.size() / 3;
  coords_.resize((curSize+nPoints)*3);
  colors_.resize((curSize+nPoints)*3);
  for (int i = 0; i < 3*nPoints; ++i) {
    coords_[curSize*3+i] = points[i];
    colors_[curSize*3+i] = colors[i]*colorScale;
  }
  SDL_UnlockMutex(lock_);
}

template <typename T>
void PointEdgeViewer::AddEdges(int nEdges, T* edges) {
  SDL_LockMutex(lock_);
  int curSize = edges_.size() / 2;
  edges_.resize((curSize+nEdges)*2);
  for (int i = 0; i < 2*nEdges; ++i) {
    edges_[curSize*2+i] = edges[i];
  }
  SDL_UnlockMutex(lock_);
}

}   // namespace xyUtils

#endif   // __XYUTILS_POINT_EDGE_VIEWER_H__
