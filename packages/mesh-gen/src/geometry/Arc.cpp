#include "geometry/Arc.hpp"
#include "geometry/GeometryUtils.hpp"

#include <algorithm>
#include <cmath>

namespace geometry {

    Arc::Arc(const Point& center_, double radius_, double startAngleDegree_, double endAngleDegree_)
        : center(center_), radius(radius_), startAngleDegree(startAngleDegree_),
          endAngleDegree(endAngleDegree_) {}

    bool Arc::isClosed() const {
        return false;
    }

    std::vector<Point> Arc::discretize(const DiscretizationOptions& options) const {
        if (radius <= 0.0) {
            return {};
        }

        const double spanDegree = ccwSpanDegrees(startAngleDegree, endAngleDegree);
        const double spanRadian = degreeToRadians(spanDegree);

        double maxAngleRadian = degreeToRadians(options.maxAngleDegree);

        if (options.maxChordError > 0.0 && radius > 0.0) {
            const double ratio = std::min(1.0, options.maxChordError / radius);
            const double chordLimitedAngle = 2.0 * std::acos(1.0 - ratio);

            maxAngleRadian = std::min(maxAngleRadian, chordLimitedAngle);
        }

        const int segments = std::max(1, static_cast<int>(std::ceil(spanRadian / maxAngleRadian)));
        std::vector<Point> points;
        points.reserve(static_cast<std::size_t>(segments) + 1);

        for (int i = 0; i <= segments; ++i) {
            const double delta = static_cast<double>(i) / static_cast<double>(segments);
            const double angleDegree = startAngleDegree + (delta * spanDegree);
            points.push_back(pointOnCircle(center.x, center.y, radius, angleDegree));
        }

        return points;
    }

} // namespace geometry
