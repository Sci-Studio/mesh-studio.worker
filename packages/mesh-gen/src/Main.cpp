#include "MeshGen.hpp"

#include <iostream>

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "Usage: mesh-gen <output_path>\n";
    return 1;
  }

  return generateMesh(argv[1]);
}
