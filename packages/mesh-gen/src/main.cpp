#include "mesh_gen/mesh_gen.h"

#include <iostream>

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "Usage: mesh-gen <output_path>\n";
    return 1;
  }

  return mesh_gen::write_mesh_to_file(argv[1]);
}
