#include "MeshGen.hpp"
#include "DataStructures.hpp"
#include "IO.hpp"
#include "parser/DxfParser.hpp"
#include "Delaunay.hpp"
#include "Mesh2D.hpp"

#include <fstream>
#include <iostream>
#include <string>


int writeMeshToFile(const char* output_path) {

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

int generateMesh(const char* input_path) {

  Mesh mesh;
  dxf::DxfParser parser;
  const char* svg_path = "delaunay.svg";

  if(!parser.loadDxf(input_path, mesh)) {
    std::cerr << "Failed to load mesh from " << input_path << std::endl;
    return 1;
  }

  std::cout << "Loaded " << mesh.points.size() << " points from " << input_path
            << "\n";

  if (!triangulate(mesh)) {
    std::cerr << "triangulate failed\n";
    return 2;
  }

  std::cout << "Triangles: " << mesh.triangles.size() << "\n";
  for (size_t i = 0; i < mesh.triangles.size(); ++i) {
    const Triangle& t = mesh.triangles[i];
    std::cout << "  [" << i << "] (" << t.v[0] << ", " << t.v[1] << ", "
              << t.v[2] << ")\n";
  }

  if (!allTrianglesCcw(mesh)) {
    std::cerr << "Validation failed: not all triangles are CCW\n";
    return 3;
  }
  if (!isDelaunay(mesh)) {
    std::cerr << "Validation failed: not Delaunay\n";
    return 4;
  }

  std::cout << "Validation OK (CCW + Delaunay)\n";

  if (!writeSvg(mesh, svg_path)) {
    std::cerr << "Failed to write SVG: " << svg_path << "\n";
    return 5;
  }
  std::cout << "Wrote " << svg_path << "\n";

  return 0;
}

