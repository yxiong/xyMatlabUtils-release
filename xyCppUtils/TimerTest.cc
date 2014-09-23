/**
  * Description.
  *
  * Author: Ying Xiong.
  * Created: Apr 30, 2013.
  */

#include "Timer.h"

#include <cmath>

#include "LogAndCheck.h"

using namespace xyUtils;

int main()  {
  Timer timer;
  
  LOG(INFO) << "Test on Timer ...";
  double tol = 1e-2;  // Accuracy tolerance, in second.
  double time1 = 0.123, time2 = 0.105, time3 = 0.111;
  Timer timer2;
  Timer::sleep(time1);
  double time1_record = timer2.elapsed();
  CHECK_NEAR(time1, time1_record, tol);
  Timer::sleep(time2);
  double time2_record = timer2.elapsed();
  CHECK_NEAR(time1 + time2, time2_record, tol);

  timer2.reset();
  Timer::sleep(time3);
  double time3_record = timer2.elapsed();
  CHECK_NEAR(time3, time3_record, tol);

  LOG(INFO) << "Passed. [" << timer.elapsed() << " seconds]";
  return 0;
}
