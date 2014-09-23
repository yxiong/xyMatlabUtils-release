/**
  * Implementation of SDLViewer abstract class.
  *
  * Author: Ying Xiong.
  * Created: Jun 15, 2013.
  */

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include "SDLViewer.h"

#include "Image.h"
#include "LogAndCheck.h"

namespace xyUtils  {
SDLViewer::SDLViewer()
    : surface_(NULL), event_thread_(NULL), render_requested_(false) {
  lock_ = SDL_CreateMutex();
  if (lock_ == NULL) {
    LOG(FATAL) << "Unable to create mutex: " << SDL_GetError();
  }
}

SDLViewer::~SDLViewer() {
  if (event_thread_)    SDL_WaitThread(event_thread_, NULL);
  SDL_DestroyMutex(lock_);
  SDL_FreeSurface(surface_);
  SDL_Quit();
}

void SDLViewer::Launch() {
  if (event_thread_ == NULL) {
    event_thread_ = SDL_CreateThread(SDLViewer::EventLoopWrapper, this);
    if (event_thread_ == NULL)
      LOG(FATAL) << "Unable to create thread: %s\n" << SDL_GetError();
  }
}

int SDLViewer::EventLoopWrapper(void* viewer) {
  reinterpret_cast<SDLViewer*>(viewer)->EventLoop();    
  return 0;
}

void SDLViewer::EventLoop() {
  Init();

  SDL_Event event;
  while (SDL_WaitEvent(&event) >= 0) {
    HandleEvent(event);
  }
}

void SDLViewer::RequestRender() {
  if (!render_requested_) {
    render_requested_ = true;
    // User event to force a render
    SDL_Event render_event;
    render_event.type = SDL_USEREVENT;
    render_event.user.code = kRenderEvent;
    render_event.user.data1 = NULL;
    render_event.user.data2 = NULL;
    // Thread safe push event
    SDL_PushEvent(&render_event);
  }
}

bool SDLViewer::HandleEvent(SDL_Event event) {
  SDL_LockMutex(lock_);
  bool handled = false;
  switch (event.type) {
    case SDL_KEYDOWN: {
      // Keyboard event.
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        // Keyboard 'esc' to exit.
        exit(0);
        handled = true;
        break;
      }
      break;
    }
    case SDL_VIDEOEXPOSE: {
      // Render event.
      Render();
      handled = true;
      break;
    }
    case SDL_USEREVENT: {
      switch (event.user.code) {
        case kRenderEvent:
          // Render event.
          Render();
          handled = true;
          break;
        case kScreenshotEvent:
          // Screenshot event.
          JpegSpec* data = static_cast<JpegSpec*>(event.user.data1);
          ScreenshotToJpegFile_(data->filename.c_str(), data->quality);
          delete data;
          handled = true;
          break;
      }
      break;
    }
    case SDL_QUIT: {
      // Window 'x' to close.
      exit(0);
      handled = true;
      break;
    }
  }
  SDL_UnlockMutex(lock_);
  return handled;
}

void SDLViewer::ScreenshotToJpegFile_(const char* filename, int quality) {
  // Note that the y-axis direction of OpenGL coordinate system is upwards,
  // while that of the 'Image' class is downwards.
  Image_8u image;
  image.SetSize(windowWidth_, windowHeight_, 3);
  unsigned char* tmp = new unsigned char[windowWidth_ * windowHeight_ * 3];
  glReadPixels(0, 0, windowWidth_, windowHeight_, GL_RGB, GL_UNSIGNED_BYTE,
               tmp);
  int stride = windowWidth_ * 3;
  for (int i = 0; i < windowHeight_; ++i) {
    memcpy(image.PixelPr(0, i), tmp + (windowHeight_-1-i)*stride, stride);
  }
  delete[] tmp;
  image.WriteToJpegFile(filename, quality);
}
}   // namespace xyUtils
