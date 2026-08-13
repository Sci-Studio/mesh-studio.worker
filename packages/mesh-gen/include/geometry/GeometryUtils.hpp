#pragma once

#include "Point.hpp"

#include <vector>
#include <numbers>

namespace geometry {

    constexpr double kDegreeToRadian = std::numbers::pi_v<double> / 180.0;

    struct BoundingBox {
        double minX, maxX;
        double minY, maxY;
    };
    
    BoundingBox getboundingBox(const std::vector<Point>& points);
    double ccwSpanDegrees(const double& startAngleDegree, const double& endAngleDegree);
    double degreeToRadians(const double& degree);
    Point pointOnCircle(const double& centerX, const double& centerY, const double& radius, const double& angleDegree);
}

