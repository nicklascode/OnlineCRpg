# Build Instructions

To build the project using CMake and MinGW:

1. Configure the build:
   
   cmake -B ./build -G "MinGW Makefiles"

2. Build the project:
   
   cmake --build build

## Multiple Build Folders

You can use any build folder name/location. For example:

cmake -B ./build-win -G "MinGW Makefiles"
cmake --build build-win

cmake -B ./build-msys -G "MinGW Makefiles"
cmake --build build-msys

CMake will generate build files in the folder you specify with `-B`. This allows you to have multiple build folders for different environments or machines without changing source paths.

**Tip:** Add build folders to your `.gitignore` so they are not tracked by Git.
