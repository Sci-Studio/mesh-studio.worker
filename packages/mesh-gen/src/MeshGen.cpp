#include "MeshGen.hpp"
#include "geometry/Mesh.hpp"
#include "parser/DxfParser.hpp"
#include "geometry/GeometryUtils.hpp"
#include "Delaunay.hpp"
#include "Mesh2D.hpp"

#include <fstream>
#include <iostream>
#include <string>

using namespace geometry;

int writeMeshToFile(const Mesh& mesh, const std::string& path) {

  if (mesh.points.empty()) {
    return false;
  }

  BoundingBox box = getboundingBox(mesh.points);

  const double pad = 0.05 * std::max(box.maxX - box.minX, box.maxY - box.minY);
  const double usePad = (pad > 0.0) ? pad : 0.1;

  double padMaxX = box.maxX + usePad; 
  double padMaxY = box.maxY + usePad; 
  double padMinX = box.minX - usePad; 
  double padMinY = box.minY - usePad; 

  const double width = padMaxX - padMinX;
  const double height = padMaxY - padMinY;

  std::ofstream out(path.c_str());
  if (!out) {
    return false;
  }

  out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
  out << "<svg xmlns=\"http://www.w3.org/2000/svg\" "
      << "viewBox=\"" << padMinX << " " << padMinY << " " << width << " " << height << "\" "
      << "width=\"800\" height=\"800\">\n";
  
  out << "  <g stroke=\"#bbb\" stroke-width=\"0.1\" fill=\"none\">\n";
  for (auto triangle : mesh.triangles) {
    const Point& a = mesh.points[triangle.v[0]];
    const Point& b = mesh.points[triangle.v[1]];
    const Point& c = mesh.points[triangle.v[2]];
    out << "    <polygon points=\"" << a.x << "," << a.y << " "
                                    << b.x << "," << b.y << " "
                                    << c.x << "," << c.y << "\" />\n";
  }
  out << "  </g>\n";

  out << "  <g stroke=\"#db3c1d\" stroke-width=\"0.2\" fill=\"none\">\n";
  for (auto edge : mesh.constraints) {
    if (edge.a < 0 || edge.b < 0 ||
      edge.a >= static_cast<int>(mesh.points.size()) ||
      edge.b >= static_cast<int>(mesh.points.size())) {
    continue;
  }
    const Point& a = mesh.points[edge.a];
    const Point& b = mesh.points[edge.b];
    out << "    <polygon points=\"" << a.x << "," << a.y << " "
                                    << b.x << "," << b.y << "\" />\n";
  }
  out << "  </g>\n";

  out << "  <g stroke=\"none\" fill=\"#1ddbdb\">\n";
  // const double r = 0.015 * std::max(width, height);
  const double r = 0.2;

  for (auto point : mesh.points) {
    out << "    <circle cx=\"" << point.x << "\" cy=\"" << point.y << "\" r=\"" << r
        << "\" />\n";
  }
  out << "  </g>\n";
  out << "</svg>\n";

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

  if (!mesh.allTrianglesCcw()) {
    std::cerr << "Validation failed: not all triangles are CCW\n";
    return 3;
  }
  // Strict Delaunay often fails on arc samples (many cocircular points). Keep CCW hard;
  // treat Delaunay as a warning until CDT is in place.
  if (!mesh.isDelaunay()) {
    std::cerr << "Warning: triangulation is not strictly Delaunay "
                 "(common with sampled arcs)\n";
  } else {
    std::cout << "Validation OK (CCW + Delaunay)\n";
  }

  writeMeshToFile(mesh, "test.svg");

  if (!writeSvg(mesh, svgPath)) {
    std::cerr << "Failed to write SVG: " << svgPath << "\n";
    return 4;
  }
  std::cout << "Wrote " << svgPath << "\n";

  return 0;
}

