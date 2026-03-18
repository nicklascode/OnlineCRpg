# Build Instructions

To build the project using CMake and MinGW:

1. Delete build folder :D

2. Configure the build:
   
   [AUTO]
   cmake -S . -B build

   [MinGW]
   cmake -S . -B build -G "MinGW Makefiles"

   [MinGW SchoolPC]
   cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_C_COMPILER=C:/msys64/mingw64/bin/gcc.exe -DCMAKE_CXX_COMPILER=C:/msys64/mingw64/bin/g++.exe

3. Build the project:
   
   cmake --build build
