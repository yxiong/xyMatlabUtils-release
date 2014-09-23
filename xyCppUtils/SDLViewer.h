/**
  * A visualizer implemented in SDL. This is an abstract class, and should be
  * inherited by users, who will need to implement 'Init' and 'Render'
  * functions, as well as the destructor (see comments below). Users usually
  * also need to implement 'HandleEvent' function for additional features.
  *
  * Note that on Mac OS X, the binary should be linked with SDLMain.o (contained
  * in libSDLMain.a), and the main function should be in the form
  *   int main(int argc, char** argv)
  * even if 'argc' and 'argv' are not used.
  *
  * Author: Ying Xiong.
  * Created: Jun 15, 2013.
  */

#ifndef __XYUTILS_SDL_VIEWER_H__
#define __XYUTILS_SDL_VIEWER_H__

#include <SDL/SDL.h>

#include "LogAndCheck.h"

namespace xyUtils  {
class SDLViewer {
 public:
  // Constructor.
  SDLViewer();
  // Destructor. Wait until the event thread exists, then clean up. Note that
  // the users have to implement their own destructor, and containing the
  // statement
  //     if (event_thread_) SDL_WaitThread(event_thread_, NULL);
  // to make sure the derived class is not destroyed when the main thread
  // terminates while the SDL thread is still alive.
  virtual ~SDLViewer();
  // Launch a new SDL window.
  void Launch();
  // Make a screenshot and store in a jpeg image.
  template<typename T> void ScreenshotToJpegFile(T filename, int quality) const;
  
 protected:
  // Static event loop wrapper for thread creation.
  static int EventLoopWrapper(void* viewer);
  // Main render loop. Executed by the event thread until interrupt or killed.
  void EventLoop();
  // Request that the screen update shortly.
  void RequestRender();
  
  // Initialize the SDL window.
  virtual void Init() = 0;
  // Main event handler. Return true if the 'event' is handled.
  virtual bool HandleEvent(SDL_Event event);
  // Render to the screen.
  virtual void Render() = 0;
  
  // Window width and height.
  int windowWidth_;
  int windowHeight_;

  // Rendering surface.
  SDL_Surface* surface_;
  // Event handler for interactivity.
  SDL_Thread* event_thread_;
  // Syncrhonization lock.
  SDL_mutex* lock_;
  // Request flag.
  bool render_requested_;

  // User event code.
  enum UserEventCode {
    kRenderEvent, kScreenshotEvent
  };
  // The data struct holding jpeg screenshot specification.
  struct JpegSpec {
    template <typename T>
    JpegSpec(T _filename, int _quality)
        : filename(_filename), quality(_quality) { }
    std::string filename;
    int quality;
  };
  // Actually make the snapshot, in SDL thread.
  void ScreenshotToJpegFile_(const char* filename, int quality);
};
// ================================================================
// Implementation of template functions.
// ================================================================
template<typename T>
void SDLViewer::ScreenshotToJpegFile(T filename, int quality) const {
  JpegSpec* data = new JpegSpec(filename, quality);
  SDL_Event screenshot_event;
  screenshot_event.type = SDL_USEREVENT;
  screenshot_event.user.code = kScreenshotEvent;
  screenshot_event.user.data1 = data;
  screenshot_event.user.data2 = NULL;
  SDL_PushEvent(&screenshot_event);
}

}   // namespace xyUtils

#endif   // __XYUTILS_SDL_VIEWER_H__
