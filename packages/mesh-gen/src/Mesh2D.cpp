#include "geometry/Mesh.hpp"

#include <fstream>

using namespace geometry;

bool writeSvg(const Mesh& mesh, const std::string& path) {
  
  if (mesh.points.empty()) {
    return false;
  }

  double minX = mesh.points[0].x;
  double minY = mesh.points[0].y;
  double maxX = minX;
  double maxY = minY;

  for (size_t i = 1; i < mesh.points.size(); ++i) {
    const Point& p = mesh.points[i];
    if (p.x < minX) minX = p.x;
    if (p.y < minY) minY = p.y;
    if (p.x > maxX) maxX = p.x;
    if (p.y > maxY) maxY = p.y;
  }

  const double pad = 0.05 * std::max(maxX - minX, maxY - minY);
  const double usePad = (pad > 0.0) ? pad : 0.1;

  minX -= usePad;
  minY -= usePad;
  maxX += usePad;
  maxY += usePad;

  const double width = maxX - minX;
  const double height = maxY - minY;

  std::ofstream out(path.c_str());
  if (!out) {
    return false;
  }

  out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
  out << "<svg xmlns=\"http://www.w3.org/2000/svg\" "
      << "viewBox=\"" << minX << " " << minY << " " << width << " " << height
      << "\" width=\"800\" height=\"800\">\n";

  const double strokeTri = 0.005 * std::max(width, height);
  const double strokeConstraint = 0.012 * std::max(width, height);

  // Triangulation (light)
  out << "  <g stroke=\"#bbb\" stroke-width=\"" << strokeTri
      << "\" fill=\"none\">\n";
  for (size_t i = 0; i < mesh.triangles.size(); ++i) {
    const Triangle& t = mesh.triangles[i];
    const Point& a = mesh.points[t.vertices[0]];
    const Point& b = mesh.points[t.vertices[1]];
    const Point& c = mesh.points[t.vertices[2]];
    out << "    <polygon points=\"" << a.x << "," << a.y << " " << b.x << ","
        << b.y << " " << c.x << "," << c.y << "\" />\n";
  }
  out << "  </g>\n";

  // Constraint edges from DXF geometry (e.g. LINE)
  out << "  <g stroke=\"#1a5fb4\" stroke-width=\"" << strokeConstraint
      << "\" fill=\"none\">\n";
  for (size_t i = 0; i < mesh.constraints.size(); ++i) {
    const Edge& e = mesh.constraints[i];
    if (e.a < 0 || e.b < 0 ||
        e.a >= static_cast<int>(mesh.points.size()) ||
        e.b >= static_cast<int>(mesh.points.size())) {
      continue;
    }
    const Point& a = mesh.points[e.a];
    const Point& b = mesh.points[e.b];
    out << "    <line x1=\"" << a.x << "\" y1=\"" << a.y << "\" x2=\"" << b.x
        << "\" y2=\"" << b.y << "\" />\n";
  }
  out << "  </g>\n";

  out << "  <g fill=\"#c44\" stroke=\"none\">\n";
  const double r = 0.015 * std::max(width, height);
  for (size_t i = 0; i < mesh.points.size(); ++i) {
    const Point& p = mesh.points[i];
    out << "    <circle cx=\"" << p.x << "\" cy=\"" << p.y << "\" r=\"" << r
        << "\" />\n";
  }
  out << "  </g>\n";
  out << "</svg>\n";
  return static_cast<bool>(out);
}
