#include "geometry/Mesh.hpp"
#include "geometry/Predicates.hpp"
namespace geometry {

  void Mesh::clear() {
    points.clear();
    triangles.clear();
    constraints.clear();
  }

  bool Mesh::allTrianglesCcw() {
    for (size_t i = 0; i < this->triangles.size(); ++i) {
      const Triangle& t = this->triangles[i];
      const Point& a = this->points[t.v[0]];
      const Point& b = this->points[t.v[1]];
      const Point& c = this->points[t.v[2]];
      if (orient2D(a, b, c) <= 0.0) {
        return false;
      }
    }
    return true;
  }
  
  bool Mesh::isDelaunay() {
    const int n = static_cast<int>(this->points.size());
    for (size_t ti = 0; ti < this->triangles.size(); ++ti) {
      const Triangle& t = this->triangles[ti];
      const Point& a = this->points[t.v[0]];
      const Point& b = this->points[t.v[1]];
      const Point& c = this->points[t.v[2]];
      for (int pi = 0; pi < n; ++pi) {
        if (pi == t.v[0] || pi == t.v[1] || pi == t.v[2]) {
          continue;
        }
        if (inCircle(a, b, c, this->points[pi]) > 0.0) {
          return false;
        }
      }
    }
    return true;
  }
}  