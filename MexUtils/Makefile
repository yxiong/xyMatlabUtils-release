# Makefile
# USAGE:
#   'make MATLAB_ROOT=/path/to/matlab [release/debug]'
#                              # compile the debug or release version
#   'make clean'               # removes .o and executable files
#
# NOTE: Do not call this Makefile from command line directly, call it from
# Matlab script 'compileMexUtils.m'.
#
#   Author: Ying Xiong.
#   Modified: Dec 16, 2012.

# Targets.
TARGET = libxymex.a
OBJS = MexIO.o

# Compilers.
CC ?= gcc
CXX ?= g++

# Compiler flags.
CFLAGS = -W -Wall -Wextra -fPIC

# Include directories.
INCLUDES = -I$(MATLAB_ROOT)/extern/include/

# Targets and rules.
release: CFLAGS += -O2
release: $(TARGET)

debug: CFLAGS += -g3
debug: $(TARGET)

$(TARGET): $(OBJS)
	ar r $@ $^

%.o: %.cc
	$(CXX) $(CFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	rm -f *.o *~ $(TARGET)

.PHONY: release debug clean
