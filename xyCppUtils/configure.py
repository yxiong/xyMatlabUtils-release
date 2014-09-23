"""Configuration file for the porject, used to generate the makefile.

Usage example:
  $ python configure.py   # Configure with default libraries enabled.
  $ python configure.py --no-jpeg
  $ python configure.py --no-third-party --no-external
  $ python configure.py --no-third-party --no-external --with-jpeg
  $ python configure.py --CC=clang --CXX=clang++ --CXXFLAGS="-stdlib=libstdc++" \\
                        --use_tr1   # For cuda compatible library, see README.txt.
"""

# Author: Ying Xiong.
# Created: Aug 14, 2012.
# Updated: Nov 07, 2013; Apr 15, 2014.

import argparse
import os
from makefile_data import *

# Global variables.
thirdPartyLibs = set(["sdl", "eigen", "jpeg", "png"])
externalLibs = set(["sdl"])
allLibs = thirdPartyLibs | externalLibs
defaultLibs = set(["eigen", "jpeg", "png"])

# Parse command line input arguments.
def _parse_input():
    parser = argparse.ArgumentParser(
        usage = "python configure.py [--no-lib1 --with-lib2]",
        description = __doc__,
        formatter_class = argparse.RawDescriptionHelpFormatter)
    # Make file specifications.
    parser.add_argument("--CC", default="gcc",
                        help="C compiler.")
    parser.add_argument("--CXX", default="g++",
                        help="C++ compiler.")
    parser.add_argument("--CXXFLAGS", default = "",
                        help="CXXFLAGS.")
    parser.add_argument("--use_tr1", default=False, action="store_true",
                        help="Use std::tr1.")
    # Add "--no-third-party" and "--no-external" options.
    parser.add_argument("--no-third-party", default=False, action="store_true",
                        help="Build without libraries %s." % \
                        ", ".join(["'%s'" % l for l in thirdPartyLibs]))
    parser.add_argument("--no-external", default=False, action="store_true",
                        help="Build without libraries %s." % \
                        ", ".join(["'%s'" % l for l in externalLibs]))
    # Add "--with-lib" and "--no-lib" options.
    for lib in allLibs:
        group = parser.add_mutually_exclusive_group()
        group.add_argument("--with-" + lib, default=False, action="store_true",
                           help="Build with '" + lib + "' library.")
        group.add_argument("--no-" + lib, default=False, action="store_true",
                           help="Build without '" + lib + "' library.")

    # Do the parsing.
    args = parser.parse_args()
    return args

# Find libraries to build with and not to build with.
def _findLibs(args):
    # Analyze parsing results.
    argsDict = vars(args)
    withLibs = defaultLibs.copy()
    if (argsDict["no_third_party"]):
        withLibs -= thirdPartyLibs
    if (argsDict["no_external"]):
        withLibs -= externalLibs
    for lib in allLibs:
        if (argsDict["no_" + lib]):
            withLibs.discard(lib)
        elif (argsDict["with_" + lib]):
            withLibs.add(lib)
    noLibs = allLibs - withLibs

    # Print information.
    print "Generating makefile with\n" + \
      "  enabled libraries %s;\n" % str(withLibs) + \
      "  disabled libraries %s." % str(noLibs)

    return (withLibs, noLibs)

def writeSectionHeader(makefile, text):
    makefile.write(cmt_ln_sharp)
    makefile.write("# %s.\n" % text)
    makefile.write(cmt_ln_sharp)

if __name__ == "__main__":
    # Parse argument and find libraries to or not to build with.
    args = _parse_input()
    (withLibs, noLibs) = _findLibs(args)

    # Create the makefile.
    makefile = open("Makefile", "w")

    # Get all objects, tests and binaries that is to be built.
    objs = [o for o in all_objs if not (set(o[1]) & noLibs)]
    tests = [t for t in all_tests if not (set(t[1]) & noLibs)]
    bins = [b for b in all_bins if not (set(b[1]) & noLibs)]

    # Head.
    makefile.write(makefile_data_head + "\n")
    
    # Targets.
    writeSectionHeader(makefile, "Targets")
    makefile.write("LIBxy = %s\n" % libxy)
    makefile.write("OBJS = " + " ".join([o[0] for o in objs]) + "\n")
    makefile.write("TESTS = " + " ".join([t[0] for t in tests]) + "\n")
    makefile.write("BINS = " + " ".join([b[0] for b in bins]) + "\n")
    makefile.write("\n")
    
    # Configurations.
    writeSectionHeader(makefile, "Configurations")
    makefile.write(makefile_data_config(
        args.CC, args.CXX, args.CXXFLAGS, args.use_tr1))
    makefile.write("\n# More libraries.\n")
    if (withLibs & thirdPartyLibs):
        makefile.write("ThirdPartyIncPath = $(shell mkdir -p ThirdParty/include; echo `pwd`/ThirdParty/include)\n")
        makefile.write("INCLUDES += -I$(ThirdPartyIncPath)\n")
        makefile.write("ThirdPartyLibPath = $(shell mkdir -p ThirdParty/lib; echo `pwd`/ThirdParty/lib)\n"
                       "LIBPATH += -L$(ThirdPartyLibPath)\n\n")
    for l in libs:
        if l[0] in withLibs:
            makefile.write("LIB%s = %s\n" % (l[0], l[1]))
    makefile.write("\n")

    # Object files and the xy-library.
    writeSectionHeader(makefile, "Object files and the xy-library")
    cc_deps = " ".join([("$(DEPSDIR)/ThirdParty_"+l) for l in withLibs])
    makefile.write(makefile_data_build % (cc_deps,))
    makefile.write("\n")

    # Test files.
    writeSectionHeader(makefile, "Test files")
    for t in tests:
        deps = " ".join([("$(DEPSDIR)/ThirdParty_"+l) for l in t[1]])
        makefile.write("%s: %s.cc $(LIBxy) %s\n" % (t[0], t[0], deps))
        makefile.write("\t$(CXX) $(CXXFLAGS) $(DEPSFLAGS) $(DEFS) $(INCLUDES) " + \
                       "$< -o $@ $(LIBPATH) $(LIBS)")
        for l in t[1]:
            makefile.write(" $(LIB" + l + ")")
        makefile.write("\n\n")
    makefile.write("\n")

    # Binary files.
    writeSectionHeader(makefile, "Binary files")
    for b in bins:
        makefile.write("%s: %s.cc $(LIBxy)\n" % (b[0], b[0]))
        makefile.write("\t$(CXX) $(CXXFLAGS) $(DEPSFLAGS) $(DEFS) $(INCLUDES) " + \
                       "$< -o $@ $(LIBPATH) $(LIBS)")
        for l in b[1]:
            makefile.write(" $(LIB" + l + ")")
        makefile.write("\n\n")
    makefile.write("\n")

    # Third party code.
    writeSectionHeader(makefile, "Third party code")
    for l in withLibs:
        makefile.write(locals()["makefile_data_" + l] + "\n")

    # Run unit tests.
    writeSectionHeader(makefile, "Unit tests.")
    makefile.write("test: $(TESTS)\n")
    for t in [t[0] for t in tests]:
        if os.path.exists(t+".py"):
            makefile.write("\tpython %s.py\n" % t)
        else:
            makefile.write("\t./%s\n" % t)
    test_third_party = " ".join([("test_ThirdParty_"+l) for l in withLibs])
    makefile.write("\nfulltest: test " + test_third_party)
    makefile.write("\n\n")

    # Cscope.
    writeSectionHeader(makefile, "Cscope")
    makefile.write(makefile_data_cscope)
    makefile.write("\n")

    # Clean up.
    writeSectionHeader(makefile, "Clean up")
    clean_third_party = " ".join([("clean_ThirdParty_"+l) for l in withLibs])
    makefile.write(makefile_data_clean % \
                   (clean_third_party, clean_third_party + " " + test_third_party))
    makefile.write("\n")

    # Done.
    makefile.close()
    print "Done."
