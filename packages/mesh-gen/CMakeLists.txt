cmake_minimum_required(VERSION 3.14)
project(MeshGen)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0 
) 

FetchContent_MakeAvailable(googletest)

enable_testing()

# Shared library -> libMeshGen.so
add_library(MeshGen SHARED
  src/mesh_gen.cpp
  src/io.cpp
  src/delaunay.cpp
  src/predicates.cpp
  src/mesh2d.cpp
  src/utils.cpp
  src/parser/DxfParser.cpp
)

target_include_directories(MeshGen
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(MeshGen PROPERTIES
  VERSION 1.0.0
  SOVERSION 1
  OUTPUT_NAME MeshGen
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# CLI executable that links against the shared library
add_executable(meshgen src/main.cpp)
add_executable(meshgen_tests
  tests/test_utils.cpp
  tests/TestDxfParser.cpp
)

target_link_libraries(meshgen PRIVATE MeshGen)
target_link_libraries(meshgen_tests
  PRIVATE
    MeshGen
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(meshgen_tests)