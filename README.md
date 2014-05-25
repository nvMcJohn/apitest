apitest
=======

Simple comparison framework for APIs


BUILDING
========

To build, you will need a Cmake 2.8 (or later) install. C++11 features are 
used, so you will need to ensure you have at least the requested compiler
for your platform. See below for platform-specific instructions to build:


Windows (from a cmd prompt in the directory README.md is located in)
=======
    mkdir _out
    cd _out
    cmake -G "Visual Studio 11" ..
    start apitest.sln

The Debug and Release configurations are run regularly, so they should
continue work just fine for you out of the box.


Unix / OSX (from a cmd prompt in the directory README.md is located in)
=======
    mkdir _out
    cd _out
    cmake -G "Unix Makefiles" ..
    make

Default configuration builds debug, for release

    mkdir _release
    cd _release
    cmake -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" ..
    make

To run apitest (from a cmd prompt in the directory README.md is located in)
=======
    cd bin
    ./apitest

Changing Problems / Solutions
=======
- The active problem is changed by pressing the left or right arrow keys.
- The active solution is changed by pressing the up or down arrow keys.
- The active API (if on a platform that supports multiple APIs) is changed by pressing 'a'.


Notes on benchmarking
=======
If you are benchmarking from withing visual studio, you need to disable the 
debug heap. This applies to bother Release and Debug configurations. This is 
accomplished by specifying _NO_DEBUG_HEAP=1 in the environment field of the 
Debug Properties page of apitest (in Visual Studio).


Other useful notes
=======
apitest supports command line options. To see the complete list, run apitest 
with '-h' on the command line, which will print out a (hopefully) useful 
help. At the time of this writing, you can specify the active problem, solution
and api from the command line, as well as the resolution (specified as -r width height).
You can also specify benchmark mode, which will either run the full suite of tests, or 
if you only chose a single problem/solution via -p/-s will benchmark that single test.
