#include "mesh_gen/mesh_gen.h"

#include <fstream>
#include <iostream>
#include <string>

namespace mesh_gen {

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

}
