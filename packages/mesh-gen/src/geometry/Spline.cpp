#include "geometry/Spline.hpp"
#include "geometry/Predicates.hpp"

#include <cmath>
#include <stdexcept>

using namespace geometry;

std::vector<Point> Spline::discretizeSpline(const GeometryTolerance& tolerance) {
    if (this->degree != 1) {
        throw std::runtime_error(
            "Unsupported DXF spline degree");
    }

    if (areCollinear(this->controlPoints, tolerance.pointMerge)) {
        return { this->controlPoints.front(), this->controlPoints.back() };
    }

    return this->controlPoints;
}