/**
  * Utilities for timing, sleep, etc.
  *
  * Author: Ying Xiong.
  * Created: Apr 30, 2013.
  */

#ifndef __XYUTILS_TIMER_H__
#define __XYUTILS_TIMER_H__

#include <sys/time.h>
#include <unistd.h>

namespace xyUtils  {

// A timer class. Following code fragment shows an example on how to use it:
//   Timer timer;                     // Start your timer.
//   /* Do your job 1. */
//   double time1 = timer.elapsed();  // Time consumed by job 1 (in seconds).
//   /* Do your job 2. */
//   double time2 = timer.elapsed();  // Time consumed job 1 AND job 2.
//   timer.reset();                   // Reset timer.
//   /* Do your job 3. */
//   double time3 = timer.elapsed();  // Time consumed by job 3 ONLY.
class Timer {
 public:
  // Construct a Timer and start timing.
  Timer() {
    reset();
  }
  // Reset the timer.
  inline void reset() {
    gettimeofday(&time_start_, NULL);
  }
  // Return the amount of time elapsed since the last reset, in unit of seconds.
  inline double elapsed() const {
    timeval time_now, time_diff;
    gettimeofday(&time_now, NULL);
    timersub(&time_now, &time_start_, &time_diff);
    return time_diff.tv_sec + time_diff.tv_usec/1e6;
  }
  // Make the thread sleep for 't' seconds. Accurate to microseconds.
  inline static int sleep(double t) {
    return usleep(t*1e6);
  }
private:
  timeval time_start_;
};

}   // namespace xyUtils

#endif   // __XYUTILS_TIMER_H__
