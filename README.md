# Online C RPG Prototype
An attempt at making a "simple" rpg dungeon crawler with basic multiplayer in a low level language 'c'

Tho not done, I had fun trying out technologies like:
- Raylib
- Winsock2

# Build Instructions

To build the project using CMake and MinGW:

1. Delete build folder :D

2. Configure the build:
   
   [AUTO]
   cmake -S . -B build

   [MinGW]
   cmake -S . -B build -G "MinGW Makefiles"

3. Build the project:
   
   cmake --build build
