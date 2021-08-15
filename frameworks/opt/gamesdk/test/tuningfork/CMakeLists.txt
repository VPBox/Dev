cmake_minimum_required(VERSION 3.4.1)

message( STATUS "A CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Werror" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D _LIBCPP_ENABLE_THREAD_SAFETY_ANNOTATIONS -Os -fPIC" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGOOGLE_PROTOBUF_NO_RTTI -DHAVE_PTHREAD")

set(ANDROID_GTEST_DIR "../../../external/googletest")
add_subdirectory("${ANDROID_GTEST_DIR}/googletest"
  googletest-build
)

include("../../src/protobuf/protobuf.cmake")

# Save the generation dir as it gets overwritten when we add_subdirectory tuningfork
set(PGENS_DIR "${PROTO_GENS_DIR}")

protobuf_generate_full_cpp( ${CMAKE_CURRENT_SOURCE_DIR}/proto
  proto/tuningfork.proto
  proto/tuningfork_clearcut_log.proto
  proto/dev_tuningfork.proto
)

message( STATUS "B CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
add_subdirectory("../../src/tuningfork"
  tuningfork-build
)

include_directories(
  "${ANDROID_GTEST_DIR}/googletest/include"
  ../../src
  ../../src/common
  ../../include
  ../../../external/nanopb-c
  ${PGENS_DIR}
  ${PROTOBUF_SRC_DIR}
  ${PROTOBUF_SRC_DIR}/..
)

add_executable(tuningfork_test
  main.cpp
  histogram_test.cpp
  tuningfork_test.cpp
  annotation_test.cpp
  ${PGENS_DIR}/full/dev_tuningfork.pb.cc
  ${PGENS_DIR}/full/tuningfork.pb.cc
  ${PGENS_DIR}/full/tuningfork_clearcut_log.pb.cc
)
add_library( protobuf-static
  STATIC ${PROTOBUF_LITE_SRCS} ${PROTOBUF_SRCS}
)
target_compile_options(protobuf-static PUBLIC "-Wno-tautological-constant-compare" "-Wno-enum-compare-switch")

target_link_libraries(tuningfork_test
  gtest
  tuningfork_static
  protobuf-static
  log
)
