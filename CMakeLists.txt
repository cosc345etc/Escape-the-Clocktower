cmake_minimum_required(VERSION 3.13)
project(untitled C)

set(CMAKE_C_STANDARD 99)

add_executable(untitled main.c Player.h)

INCLUDE_DIRECTORIES(  /usr/local/Cellar/allegro/5.2.4.0/include )
LINK_DIRECTORIES(  /usr/local/Cellar/allegro/5.2.1.4.0/lib )

file(GLOB LIBRARIES "/usr/local/Cellar/allegro/5.2.4.0/lib/*.dylib")
message("LIBRARIES = ${LIBRARIES}")

TARGET_LINK_LIBRARIES(untitled  ${LIBRARIES})
