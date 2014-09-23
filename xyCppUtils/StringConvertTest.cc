/**
  * Test for StringConvert utilities.
  *
  * Author: Ying Xiong
  * Created: Aug 18, 2012
  */

#include "StringConvert.h"

#include <string>

#include "LogAndCheck.h"
#include "Timer.h"

using std::string;
using namespace xyUtils;

int main()  {
  Timer timer;
  LOG(INFO) << "Test on string convert utilities...";

  StringConvert::OutStatus out;

  //==========================================================
  // Test on StringConverter::ToBool().
  bool b;
  out = StringConvert::ToBool(string("True"), &b);
  CHECK_EQ(out, StringConvert::Success);
  CHECK_EQ(b, true);
  out = StringConvert::ToBool("true", &b);
  CHECK_EQ(out, StringConvert::Success);
  CHECK_EQ(b, true);
  out = StringConvert::ToBool(string("1"), &b);
  CHECK_EQ(out, StringConvert::Success);
  CHECK_EQ(b, true);
  out = StringConvert::ToBool("False", &b);
  CHECK_EQ(out, StringConvert::Success);
  CHECK_EQ(b, false);
  out = StringConvert::ToBool(string("false"), &b);
  CHECK_EQ(out, StringConvert::Success);
  CHECK_EQ(b, false);
  out = StringConvert::ToBool(string("0"), &b);
  CHECK_EQ(out, StringConvert::Success);
  CHECK_EQ(b, false);
  out = StringConvert::ToBool(string("blabla"), &b);
  CHECK_EQ(out, StringConvert::Inconvertible);
  out = StringConvert::ToBool(string("100"), &b);
  CHECK_EQ(out, StringConvert::Inconvertible);
  out = StringConvert::ToBool(string(""), &b);
  CHECK_EQ(out, StringConvert::Inconvertible);

  //==========================================================
  // Test on StringConverter::ToInt().
  int i;
  out = StringConvert::ToInt(string("-125"), &i);
  CHECK_EQ(out, StringConvert::Success);
  CHECK_EQ(i, -125);
  out = StringConvert::ToInt("0xFF", &i);
  CHECK_EQ(out, StringConvert::Success);
  CHECK_EQ(i, 0xff);
  out = StringConvert::ToInt(string("4351043578943572043682598125"), &i);
  CHECK_EQ(out, StringConvert::Overflow);
  out = StringConvert::ToInt("-2340918435924068245962894576028965", &i);
  CHECK_EQ(out, StringConvert::Underflow);
  out = StringConvert::ToInt("-12a5", &i);
  CHECK_EQ(out, StringConvert::Inconvertible);
  out = StringConvert::ToInt("", &i);
  CHECK_EQ(out, StringConvert::Inconvertible);

  //==========================================================
  // Test on StringConverter::ToLong().
  long l;
  out = StringConvert::ToLong(string("-1234352345"), &l);
  CHECK_EQ(out, StringConvert::Success);
  CHECK_EQ(l, -1234352345);
  out = StringConvert::ToLong("0xFFFFFFF", &l);
  CHECK_EQ(out, StringConvert::Success);
  CHECK_EQ(l, 0xfffffffL);
  out = StringConvert::ToLong(string("4351043578943572043682598125"), &l);
  CHECK_EQ(out, StringConvert::Overflow);
  out = StringConvert::ToLong("-+23", &l);
  CHECK_EQ(out, StringConvert::Inconvertible);
  out = StringConvert::ToLong("-", &l);
  CHECK_EQ(out, StringConvert::Inconvertible);
  out = StringConvert::ToLong("", &l);
  CHECK_EQ(out, StringConvert::Inconvertible);

  //==========================================================
  // Test on StringConverter::ToDouble().
  double d;
  out = StringConvert::ToDouble(string("-123435.24122345"), &d);
  CHECK_EQ(out, StringConvert::Success);
  CHECK_NEAR(d, -123435.24122345, 1e-8);
  out = StringConvert::ToDouble("123E+45", &d);
  CHECK_EQ(out, StringConvert::Success);
  CHECK_NEAR(d, 123E+45, 1e35);
  out = StringConvert::ToDouble(string("435e1043578943572043682598125"), &d);
  CHECK_EQ(out, StringConvert::Overflow);
  out = StringConvert::ToDouble("+-asdf23", &d);
  CHECK_EQ(out, StringConvert::Inconvertible);
  out = StringConvert::ToDouble("++++", &d);
  CHECK_EQ(out, StringConvert::Inconvertible);
  out = StringConvert::ToDouble("", &d);
  CHECK_EQ(out, StringConvert::Inconvertible);

  LOG(INFO) << "Passed. [" << timer.elapsed() << " seconds]";
  return 0;
}
