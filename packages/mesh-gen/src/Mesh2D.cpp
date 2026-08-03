#include "Mesh2D.hpp"

#include "Predicates.hpp"

#include <algorithm>
#include <fstream>



bool all_triangles_ccw(const Mesh& mesh) {
  for (size_t i = 0; i < mesh.triangles.size(); ++i) {
    const Triangle& t = mesh.triangles[i];
    const Vec2& a = mesh.points[t.v[0]];
    const Vec2& b = mesh.points[t.v[1]];
    const Vec2& c = mesh.points[t.v[2]];
    if (orient2d(a, b, c) <= 0.0) {
      return false;
    }
  }
  return true;
}

bool is_delaunay(const Mesh& mesh) {
  const int n = static_cast<int>(mesh.points.size());
  for (size_t ti = 0; ti < mesh.triangles.size(); ++ti) {
    const Triangle& t = mesh.triangles[ti];
    const Vec2& a = mesh.points[t.v[0]];
    const Vec2& b = mesh.points[t.v[1]];
    const Vec2& c = mesh.points[t.v[2]];
    for (int pi = 0; pi < n; ++pi) {
      if (pi == t.v[0] || pi == t.v[1] || pi == t.v[2]) {
        continue;
      }
      if (incircle(a, b, c, mesh.points[pi]) > 0.0) {
        return false;
      }
    }
  }
  return true;
}

bool write_svg(const Mesh& mesh, const std::string& path) {
  
  if (mesh.points.empty()) {
    return false;
  }

  double min_x = mesh.points[0].x;
  double min_y = mesh.points[0].y;
  double max_x = min_x;
  double max_y = min_y;

  for (size_t i = 1; i < mesh.points.size(); ++i) {
    const Vec2& p = mesh.points[i];
    if (p.x < min_x) min_x = p.x;
    if (p.y < min_y) min_y = p.y;
    if (p.x > max_x) max_x = p.x;
    if (p.y > max_y) max_y = p.y;
  }

  const double pad = 0.05 * std::max(max_x - min_x, max_y - min_y);
  const double use_pad = (pad > 0.0) ? pad : 0.1;

  min_x -= use_pad;
  min_y -= use_pad;
  max_x += use_pad;
  max_y += use_pad;

  const double width = max_x - min_x;
  const double height = max_y - min_y;

  std::ofstream out(path.c_str());
  if (!out) {
    return false;
  }

  out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
  out << "<svg xmlns=\"http://www.w3.org/2000/svg\" "
      << "viewBox=\"" << min_x << " " << min_y << " " << width << " " << height
      << "\" width=\"800\" height=\"800\">\n";

  // Flip Y for a more conventional math-style view (optional: keep raw coords)
  out << "  <g stroke=\"#222\" stroke-width=\"" << (0.005 * std::max(width, height))
      << "\" fill=\"none\">\n";

  for (size_t i = 0; i < mesh.triangles.size(); ++i) {
    const Triangle& t = mesh.triangles[i];
    const Vec2& a = mesh.points[t.v[0]];
    const Vec2& b = mesh.points[t.v[1]];
    const Vec2& c = mesh.points[t.v[2]];
    out << "    <polygon points=\"" << a.x << "," << a.y << " " << b.x << ","
        << b.y << " " << c.x << "," << c.y << "\" />\n";
  }

  out << "  </g>\n";
  out << "  <g fill=\"#c44\" stroke=\"none\">\n";
  const double r = 0.015 * std::max(width, height);

  for (size_t i = 0; i < mesh.points.size(); ++i) {
    const Vec2& p = mesh.points[i];
    out << "    <circle cx=\"" << p.x << "\" cy=\"" << p.y << "\" r=\"" << r
        << "\" />\n";
  }

  out << "  </g>\n";
  out << "</svg>\n";
  return static_cast<bool>(out);
}
