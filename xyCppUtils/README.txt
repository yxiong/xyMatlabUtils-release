================================================================
xyCppUtils --- Cpp utilities by Ying Xiong.
================================================================

Author: Ying Xiong.
Created: Aug 09, 2012.
Release: Sep 23, 2014 (v0.0.1).

================================================================
Prerequisite libraries.
================================================================
Building essentials:
  MacOSX: [Earlier than Mavericks] Xcode->Preferences->
          Downloads and install component named "Command Line Tools".
          [Mavericks or later] "xcode-select --install".
  Linux: "sudo apt-get install build-essentials".

libsdl (http://www.libsdl.org/) [optional]:
  MacOSX: Download the .dmg file from the website and install it.
  Linux: "sudo apt-get install libsdl1.2-dev".
  Note: we are currently using the historic version 1.2.

================================================================
Compile and run.
================================================================
For a quick build and test:
  $ python configure.py    # Create the Makefile.
  $ make                   # For the first time and some third-party libraries
                           # are enabled, this might take a while.
  $ make test              # Run all the unit tests. Use "make fulltest" to also
                           # run test on third party libraries.

To generate the essential library without any external dependencies:
  $ python configure --no-external --no-third-party
  $ python build_utils.py build-and-test all

To build the library only:
  python configure.py            # Generate the Makefile.
  make RELEASE=1 libxyutils.a    # Build the library.

To clean up everything including the third party libraries:
  $ make fullclean
  # After this, "git status --ignored" should only show a Makefile.

To generate a template file set (XXX.h, XXX.cc, XXXTest.cc):
  python generate_template_sources.py XXX

To generate the cscope tags:
  make cscope

================================================================
Naming conventions.
================================================================
File names:
  For files that define a class or a set of utility functions, the file name
  should be something like:
      SomeExitingClass.h
      VeryUsefulFunctions.cc
  For files that generate a binary command, the the file name should be
  something like:
      runSomeCommand.cc
      visualizeCertainStuff.cc

Type names:
  The type names should generally be like:
      SomeExcitingClass
      VeryUsefulStruct

Function names:
  The function names should generally be like:
      DoImportantStuff
      SetCertainValue

Variable names:
  The variable names should be like:
      name
      simpleViewer
  Class member variables should be like:
      member_
      usefulField_
  Constant variables and enumerator names should be like:
      kConstant
      kRed, kGreen, kBlue
