#include "mesh_gen.h"
#include "data_structures.h"
#include "io.h"
#include "delaunay.h"
#include <fstream>
#include <iostream>
#include <string>


int write_mesh_to_file(const char* output_path) {

  std::ofstream out(output_path, std::ios::binary | std::ios::trunc);
  if (!out) {
    std::cerr << "mesh_gen::write_mesh_to_file: failed to create file '" << output_path << "' for writing\n";
    return 2;
  }

  out << "0 0 0\n";
  if (!out) {
    std::cerr << "mesh_gen::write_mesh_to_file: failed to write to '" << output_path << "'\n";
    return 3;
  }

  out.close();
  if (!out) {
    std::cerr << "mesh_gen::write_mesh_to_file: failed to close '" << output_path << "'\n";
    return 4;
  }

  return 0;
}

int generate_mesh(const char* input_path) {

  Mesh mesh;

  if(!load_mesh(input_path, mesh)) {
    std::cerr << "Failed to load mesh from " << input_path << std::endl;
    return 1;
  }

  std::cout << "Loaded " << mesh.points.size() << " points from " << input_path
            << "\n";

  if (!triangulate(mesh)) {
    std::cerr << "triangulate failed\n";
    return 2;
  }

  return 0;
}

