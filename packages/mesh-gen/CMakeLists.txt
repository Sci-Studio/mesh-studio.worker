cmake_minimum_required(VERSION 3.5)
project(MeshGen)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Shared library -> libMeshGen.so
add_library(MeshGen SHARED
  src/mesh_gen.cpp
  src/io.cpp
  src/delaunay.cpp
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

# CLI executable that links against the shared library
add_executable(mesh-gen src/main.cpp)
target_link_libraries(mesh-gen PRIVATE MeshGen)
