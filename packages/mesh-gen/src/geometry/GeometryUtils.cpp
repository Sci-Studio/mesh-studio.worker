#include "geometry/GeometryUtils.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace geometry {

    BoundingBox getboundingBox(const std::vector<Point>& points) {
        if (points.empty()) {
            throw std::invalid_argument("Point vector is empty");
        }
    
        BoundingBox box{
            points[0].x, points[0].x,
            points[0].y, points[0].y
        };
    
        for (const auto& p : points) {
            box.minX = std::min(box.minX, p.x);
            box.maxX = std::max(box.maxX, p.x);
            box.minY = std::min(box.minY, p.y);
            box.maxY = std::max(box.maxY, p.y);
        }
        return box;
    }
    
    double degreeToRadians(const double& degree) {
        return degree * kDegreeToRadian;
    }
    
    double ccwSpanDegrees(const double& startAngleDegree, const double& endAngleDegree) {
        double spanAngleDegree = endAngleDegree - startAngleDegree;
        while (spanAngleDegree <= 0.0) {
            spanAngleDegree += 360.0;
        }
        while (spanAngleDegree > 360.0) {
            spanAngleDegree -= 360.0;
        }
        return spanAngleDegree;
    }
    
    Point pointOnCircle(const double& centerX, const double& centerY, const double& radius,
                        const double& angleDegree) {
        const double angleRadian = degreeToRadians(angleDegree);
        Point point;
        point.x = centerX + radius * std::cos(angleRadian);
        point.y = centerY + radius * std::sin(angleRadian);
        point.type = PointType::NORMAL;
        return point;
    }

}
