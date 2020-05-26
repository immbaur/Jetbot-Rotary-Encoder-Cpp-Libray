cmake_minimum_required(VERSION 3.7)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++17 -pthread")

project(rotary-encoder)

INCLUDE_DIRECTORIES(
  include
  )

# build static library "librotaryencoder.a"
add_library(rotaryencoder STATIC src/rotary_encoder.cpp src/gpiolib.cpp)

# build test app
add_executable(test-rotary-encoder test/main.cpp)
target_link_libraries(test-rotary-encoder rotaryencoder)

# install static library to /usr/local/lib
INSTALL(TARGETS rotaryencoder DESTINATION lib)
# install public rotary-encoder header to /usr/local/include/
INSTALL(DIRECTORY include/rotary-encoder DESTINATION include)