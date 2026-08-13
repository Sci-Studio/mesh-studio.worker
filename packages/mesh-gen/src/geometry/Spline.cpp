#include "geometry/Spline.hpp"
#include "geometry/Predicates.hpp"

#include <cmath>
#include <stdexcept>

namespace geometry {

  bool Spline::isClosed() const {
    if (controlPoints.size() < 3) {
      return false;
    }
    const Point& a = controlPoints.front();
    const Point& b = controlPoints.back();
    constexpr double eps = 1e-8;
    return std::abs(a.x - b.x) <= eps && std::abs(a.y - b.y) <= eps;
  }

  std::vector<Point> Spline::discretize(const DiscretizationOptions& options) const {
    if (degree != 1) {
      throw std::runtime_error("Unsupported DXF spline degree");
    } 

    if (controlPoints.size() < 2) {
      return controlPoints;
    }

    if (areCollinear(controlPoints, options.pointMerge)) {
      if (isClosed()) {
        return {controlPoints.front()};
      }
      return {controlPoints.front(), controlPoints.back()};
    }

    if (isClosed()) {
      return std::vector<Point>(controlPoints.begin(), controlPoints.end() - 1);
    } 
    return controlPoints;
  }

} 
