cmake_minimum_required(VERSION 3.13)
project(Escape-the-Clocktower-Beta C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")
file(COPY ETC_5X1_CORRIDOR_GFX DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_executable(Escape-the-Clocktower-Beta src/main.c src/Player.h)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
target_link_libraries(Escape-the-Clocktower-Beta ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})