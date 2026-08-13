#include "geometry/Mesh.hpp"

namespace geometry {

  void Mesh::clear() {
    points.clear();
    triangles.clear();
    constraints.clear();
  }

}  