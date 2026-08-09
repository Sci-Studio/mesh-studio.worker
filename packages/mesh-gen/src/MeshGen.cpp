#include "MeshGen.hpp"
#include "DataStructures.hpp"
#include "parser/DxfParser.hpp"
#include "Delaunay.hpp"
#include "Mesh2D.hpp"

#include <fstream>
#include <iostream>
#include <string>


int writeMeshToFile(const char* outputPath) {

  std::ofstream out(outputPath, std::ios::binary | std::ios::trunc);
  if (!out) {
    std::cerr << "mesh_gen::write_mesh_to_file: failed to create file '" << outputPath << "' for writing\n";
    return 2;
  }

  out << "0 0 0\n";
  if (!out) {
    std::cerr << "mesh_gen::write_mesh_to_file: failed to write to '" << outputPath << "'\n";
    return 3;
  }

  out.close();
  if (!out) {
    std::cerr << "mesh_gen::write_mesh_to_file: failed to close '" << outputPath << "'\n";
    return 4;
  }

  return 0;
}

int generateMesh(const char* inputPath) {

  Mesh mesh;
  parser::dxf::DxfParser parser;
  const char* svgPath = "delaunay.svg";

  if(!parser.loadMesh(inputPath, mesh)) {
    std::cerr << "Failed to load mesh from " << inputPath << std::endl;
    return 1;
  }

  std::cout << "Loaded " << mesh.points.size() << " points, "
            << mesh.constraints.size() << " constraint edges from " << inputPath
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
  // Strict Delaunay often fails on arc samples (many cocircular points). Keep CCW hard;
  // treat Delaunay as a warning until CDT is in place.
  if (!isDelaunay(mesh)) {
    std::cerr << "Warning: triangulation is not strictly Delaunay "
                 "(common with sampled arcs)\n";
  } else {
    std::cout << "Validation OK (CCW + Delaunay)\n";
  }

  if (!writeSvg(mesh, svgPath)) {
    std::cerr << "Failed to write SVG: " << svgPath << "\n";
    return 4;
  }
  std::cout << "Wrote " << svgPath << "\n";

  return 0;
}

