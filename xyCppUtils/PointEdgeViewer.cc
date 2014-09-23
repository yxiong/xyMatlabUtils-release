/**
  * Implementation of PointEdgeViewer class.
  *
  * Author: Ying Xiong.
  * Created: Jun 13, 2013.
  */

#include "PointEdgeViewer.h"

#include <Eigen/Core>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include "LogAndCheck.h"

using namespace Eigen;

namespace xyUtils  {

PointEdgeViewer::PointEdgeViewer()
    : SDLViewer(),
      camera_(Vector3d(0, 0, 0), Vector3d(0, 0, 1), 1, Vector3d(0, 1, 0)) {
}

void PointEdgeViewer::Clear() {
  SDL_LockMutex(lock_);
  coords_.clear();
  colors_.clear();
  edges_.clear();
  RequestRender();
  SDL_UnlockMutex(lock_);
}

void PointEdgeViewer::CenterView() {
  SDL_LockMutex(lock_);
  int n = coords_.size() / 3;
  double x = 0, y = 0, z = 0;
  for (int i = 0; i < n; ++i) {
    x += coords_[3*i + 0];
    y += coords_[3*i + 1];
    z += coords_[3*i + 2];
  }
  camera_.SetLookat(Vector3d(x/n, y/n, z/n));
  RequestRender();
  SDL_UnlockMutex(lock_);
}

void PointEdgeViewer::SetCamDist(double dist) {
  SDL_LockMutex(lock_);
  camera_.SetCamDist(dist);
  RequestRender();
  SDL_UnlockMutex(lock_);
}

void PointEdgeViewer::RotateCamAcrossUp(double ang) {
  SDL_LockMutex(lock_);
  camera_.RotateAcrossUp(ang);
  RequestRender();
  SDL_UnlockMutex(lock_);
}

void PointEdgeViewer::RotateCamAcrossRight(double ang) {
  SDL_LockMutex(lock_);
  camera_.RotateAcrossRight(ang);
  RequestRender();
  SDL_UnlockMutex(lock_);
}

void PointEdgeViewer::Init() {
  SDL_LockMutex(lock_);

  // Set X11 Driver.
  std::string driver = "SDL_VIDEODRIVER=x11";
  SDL_putenv((char*) driver.c_str());

  // Initialize SDL.
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    LOG(FATAL) << "Init Error: %s\n" << SDL_GetError();
  }

  // Enable key repeat.
  if (SDL_EnableKeyRepeat(500, 200) < 0) {
    LOG(FATAL) << "EnableKeyRepeat Error: %s\n" << SDL_GetError();
  }

  windowWidth_ = 640;
  windowHeight_ = 480;
  int bits_per_pixel = 32;

  // Create the window and rendering surface
  surface_ = SDL_SetVideoMode(windowWidth_, windowHeight_, bits_per_pixel,
                              SDL_HWSURFACE|SDL_GL_DOUBLEBUFFER|SDL_OPENGL);
  if (surface_ == NULL) {
    LOG(FATAL) << "Display Error: %s\n" << SDL_GetError();
  }

  // Background Color.
  glClearColor(0.0, 0.0, 0.0, 1.0);
  // Initialize View.
  glViewport(0, 0, windowWidth_, windowHeight_);

  // Set projective matrix.
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, float(windowWidth_) / float(windowHeight_),
                 0.01, 1000.0);

  // Point system.
  glEnable(GL_POINT_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_DEPTH_TEST);

  SDL_UnlockMutex(lock_);
}

bool PointEdgeViewer::HandleEvent(SDL_Event event) {
  if (SDLViewer::HandleEvent(event)) {
    return true;
  }
  SDL_LockMutex(lock_);
  bool handled = false;
  switch (event.type) {
    case SDL_KEYDOWN: {
      // Keyboard event.
      switch (event.key.keysym.sym) {
        case SDLK_c:
          // Keyboard 'c' to center.
          CenterView();
          RequestRender();
          handled = true;
          break;
        default:
          // Do nothing for other keys.
          ;
      }
      break;
    }
    case SDL_MOUSEMOTION: {
      // Mouse motion event.
      if (event.motion.state == SDL_BUTTON(1)) {
        // Left mouse button is down.
        camera_.RotateAcrossRight(-0.01*event.motion.yrel);
        camera_.RotateAcrossUp(-0.01*event.motion.xrel);
        RequestRender();
        handled = true;
      } else if (event.motion.state == SDL_BUTTON(3)) {
        // Right mouse button is down.
        camera_.TranslateRightUp(-0.005*event.motion.xrel, 0.005*event.motion.yrel);
        RequestRender();
        handled = true;
      }
      break;
    }
    case SDL_MOUSEBUTTONDOWN: {
      // Wheel event.
      if (event.button.button == SDL_BUTTON_WHEELUP) {
        camera_.Zoom(1.25);
        RequestRender();
        handled = true;
      } else if (event.button.button == SDL_BUTTON_WHEELDOWN) {
        camera_.Zoom(0.8);
        RequestRender();
        handled = true;
      }
      break;
    }
  }
  SDL_UnlockMutex(lock_);
  return handled;
}

void PointEdgeViewer::Render() {
  SDL_LockMutex(lock_);
  DrawPointsAndEdges();
  render_requested_ = false;
  SDL_UnlockMutex(lock_);
}

void PointEdgeViewer::DrawPointsAndEdges() {
  // Clear the screen and z-buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Set view matrix.
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  Vector3d eye = camera_.GetCamPosition();
  Vector3d lookat = camera_.GetLookat();
  Vector3d up = camera_.GetUp();
  gluLookAt(eye[0], eye[1], eye[2],
            lookat[0], lookat[1], lookat[2],
            up[0], up[1], up[2]);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);

  // Add points and colors.
  glVertexPointer(3, coord_type, 0, coords_.data());
  glColorPointer(3, color_type, 0, colors_.data());

  // Draw the points.
  glPointSize(1.5);
  glDrawArrays(GL_POINTS, 0, coords_.size() / 3);

  // Draw the edges.
  glLineWidth(1);
  glDrawElements(GL_LINES, edges_.size(), index_type, edges_.data());

  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);

  // Make visible.
  SDL_GL_SwapBuffers();
}
}   // namespace xyUtils
