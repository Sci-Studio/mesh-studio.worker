#include "geometry/Circle.hpp"
#include "geometry/GeometryUtils.hpp"

#include <algorithm>
#include <cmath>

namespace geometry {

    Circle::Circle(const Point& center_, double radius_) : center(center_), radius(radius_) {}

    bool Circle::isClosed() const {
        return true;
    }

    std::vector<Point> Circle::discretize(const DiscretizationOptions& options) const {
        if (radius <= 0.0) {
            return {};
        }

        constexpr double spanDegree = 360.0;
        constexpr int minSegments = 3;
        const double spanRadian = degreeToRadians(spanDegree);

        double maxAngleRadian = degreeToRadians(options.maxAngleDegree);

        if (options.maxChordError > 0.0 && radius > 0.0) {
            const double ratio = std::min(1.0, options.maxChordError / radius);
            const double chordLimitedAngle = 2.0 * std::acos(1.0 - ratio);
            maxAngleRadian = std::min(maxAngleRadian, chordLimitedAngle);
        }

        const int segments =
            std::max(minSegments, static_cast<int>(std::ceil(spanRadian / maxAngleRadian)));

        std::vector<Point> points;
        points.reserve(static_cast<std::size_t>(segments));

        for (int i = 0; i < segments; ++i) {
            const double delta = static_cast<double>(i) / static_cast<double>(segments);
            const double angleDegree = delta * spanDegree;

            points.push_back(pointOnCircle(center.x, center.y, radius, angleDegree));
        }

        return points;
    }

} // namespace geometry
