#include "geometry/Spline.hpp"

#include <cmath>
#include <stdexcept>

using namespace geometry;

bool Spline::areCollinear(const std::vector<Point>& points, double distanceTolerance) {
    if (points.size() < 3) {
        return true;
    }

    const Point& a = points.front();
    const Point& b = points.back();

    const double dx = b.x - a.x;
    const double dy = b.y - a.y;

    const double length = std::hypot(dx, dy);

    if (length == 0.0) {
        return false;
    }

    for (auto point : points) {
        
        const double cross = dx * (point.y - a.y) - dy * (point.x - a.x);
        const double distance = std::abs(cross) / length;

        if (distance > distanceTolerance) {
            return false;
        }
    }

    return true;
}


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