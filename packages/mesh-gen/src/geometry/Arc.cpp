#include "geometry/Arc.hpp"

#include <algorithm>
#include <cmath>

using namespace geometry;

double Arc::degreeToRadians(const double& degree) {
    return degree * kDegreeToRadian;
}

double Arc::ccwSpanDegrees(const double& startAngleDegree, const double& endAngleDegree) {
    double spanAngleDegree = endAngleDegree - startAngleDegree;
    while (spanAngleDegree <= 0.0) {
        spanAngleDegree += 360.0;
    }
    while (spanAngleDegree > 360.0) {
        spanAngleDegree -= 360.0;
    }
    return spanAngleDegree;
}

Point Arc::pointOnCircle(const double& angleDegree) {
    const double angleRadian = degreeToRadians(angleDegree);
    Point point;
    point.x = this->centerX + this->radius * std::cos(angleRadian);
    point.y = this->centerY + this->radius * std::sin(angleRadian);
    point.type = PointType::NORMAL;
    return point;
}

std::vector<Point> Arc::discretizeArc(const ArcDiscretizationOption& options) {
    const double spanDegree = ccwSpanDegrees(this->startAngleDegree, this->endAngleDegree);

    const double spanRadian = degreeToRadians(spanDegree);

    double maxAngleRadian = degreeToRadians(options.maxAngleDegree);

    if (options.maxChordError > 0.0 && this->radius) {
        const double chordLimitedAngle = 2.0 * std::acos(1.0 - (options.maxChordError / this->radius));
        maxAngleRadian = std::min(maxAngleRadian, chordLimitedAngle);
    }

    const int segments = std::max(1, static_cast<int>(std::ceil(spanRadian / maxAngleRadian)));

    std::vector<Point> points;
    points.reserve(static_cast<std::size_t>(segments) + 1);

    for (int i = 0; i <= segments; ++i) {
        const double delta = static_cast<double>(i) / static_cast<double>(segments);
        const double angleDegree = this->startAngleDegree + (delta * spanDegree);

        points.push_back(pointOnCircle(angleDegree));
    }

    return points;
}