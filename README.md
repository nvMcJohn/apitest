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
cmake -g "Visual Studio 11" ..
start apitest.sln

