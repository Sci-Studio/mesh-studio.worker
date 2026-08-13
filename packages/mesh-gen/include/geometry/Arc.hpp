#pragma once

#include "Point.hpp"

#include <vector>

namespace geometry {


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
    };
}