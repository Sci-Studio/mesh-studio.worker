#pragma once

#include "Edge.hpp"
#include "Point.hpp"
#include "Triangle.hpp"

#include <vector>

namespace geometry {

  class Mesh {
    public:
      std::vector<Point> points;
      std::vector<Triangle> triangles;
      std::vector<Edge> constraints;

      void clear();
      bool allTrianglesCcw();
      bool isDelaunay();

      int addUnique(const Point& point);
            
      void addConstraintEdge(int i0, int i1);
      void addPolylineConstraints(const std::vector<Point>& polyline, bool closed);
  };

}
