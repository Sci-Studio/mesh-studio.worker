#include "geometry/Mesh.hpp"
#include "geometry/Predicates.hpp"

#include <cmath>

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

  int Mesh::addUnique(const Point& point) {
    constexpr double eps = 1e-9;

    for (auto existingPoint : points) {

      if (std::abs(existingPoint.x - point.x) <= eps &&
          std::abs(existingPoint.y - point.y) <= eps) {
            return static_cast<int>(existingPoint.index);
        }
    } 
    Point p = point;
    p.index = static_cast<int>(points.size());
    points.push_back(p);
    return p.index;
  }

  void Mesh::addConstraintEdge(int i0, int i1) {
    if (i0 != i1) {
        constraints.push_back(Edge(i0, i1));
    }
  }

  void Mesh::addPolylineConstraints(const std::vector<Point>& polyline, bool closed) {
    if (polyline.size() < 2) {
        return;
    }
    const int first = addUnique(polyline.front());
    int previous = first;
    for (size_t i = 1; i < polyline.size(); ++i) {
        const int current = addUnique(polyline[i]);
        addConstraintEdge( previous, current);
        previous = current;
    }

    if (closed && polyline.size() >= 3) {
      addConstraintEdge(previous, first);
    }
  }
}  