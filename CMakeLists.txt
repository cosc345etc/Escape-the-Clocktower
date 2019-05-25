cmake_minimum_required(VERSION 3.13)
project(etc C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")

file(COPY ETC_5X1_CORRIDOR_GFX DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_executable(etc main.c Player.h)

INCLUDE_DIRECTORIES(/usr/local/Cellar/allegro/5.2.4.0/include )
LINK_DIRECTORIES(/usr/local/Cellar/allegro/5.2.1.4.0/lib )

file(GLOB LIBRARIES "/usr/local/Cellar/allegro/5.2.4.0/lib/*.dylib")
message("LIBRARIES = ${LIBRARIES}")

TARGET_LINK_LIBRARIES(etc  ${LIBRARIES})
