cmake_minimum_required(VERSION 3.10)
project (sdl_practice)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
message("SDL IMAGE LIBRARIES:")
message("${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES}")

add_executable(sdl_practice main.cpp)
# FIXME: find_package doesn't seem to be finding SDL2_image so I've just hacked in a hard coded library path
target_link_libraries(sdl_practice ${SDL2_LIBRARIES} SDL2_image::SDL2_image)
