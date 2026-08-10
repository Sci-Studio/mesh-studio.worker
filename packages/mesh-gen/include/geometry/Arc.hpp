#pragma once

#include "Point.hpp"

#include <vector>
#include <numbers>

namespace geometry {

    constexpr double kDegreeToRadian = std::numbers::pi_v<double> / 180.0;

    struct ArcDiscretizationOption {
        double maxChordError;
        double maxAngleDegree;
    };

    class Arc {
        public:
            double centerX;
            double centerY;
            double radius;
            double startAngleDegree;
            double endAngleDegree;

            std::vector<Point> discretizeArc(const ArcDiscretizationOption& options);
            double ccwSpanDegrees(const double& startAngleDegree, const double& endAngleDegree);
            double degreeToRadians(const double& degree);
            Point pointOnCircle(const double& angleDegree);
    };
}