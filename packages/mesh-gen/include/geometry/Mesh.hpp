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
  };

}
