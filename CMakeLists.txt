cmake_minimum_required(VERSION 3.5)
project(OnlineCRpg)

include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
)
FetchContent_MakeAvailable(raylib)

add_executable(OnlineCRpg
    src/main.c
    src/level/level.c
    src/global.c
    src/network/network.c
    src/network/client.c
    src/network/server.c
    src/util/logger.c
    src/network/packet.c
    src/network/global_net.c
    src/level/tile.c
    src/util/assets.c
    src/entites/entity.c
)
target_link_libraries(OnlineCRpg PRIVATE raylib ws2_32)

# Set C standard
set_target_properties(OnlineCRpg PROPERTIES C_STANDARD 99)

# Platform-specific settings
if(WIN32)
    target_compile_definitions(OnlineCRpg PRIVATE _CRT_SECURE_NO_WARNINGS)
    if(MSVC)
        set_property(TARGET OnlineCRpg PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif()
    # Disable implicit function declaration warning for GCC/MinGW
    if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
        target_compile_options(OnlineCRpg PRIVATE -Wno-implicit-function-declaration) # Prop unsafe
    endif()
endif()

# Optionally, install target
install(TARGETS OnlineCRpg RUNTIME DESTINATION bin)