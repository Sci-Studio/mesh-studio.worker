#pragma once

#include "Point.hpp"

#include <vector>

namespace geometry {

    struct GeometryTolerance {
        double pointMerge = 1e-8;
        double chordError = 1e-2;
    };

    class Spline {
        public:
            int degree = 0;
            std::vector<Point> controlPoints;
            std::vector<double> knots;
            std::vector<double> weights;

            std::vector<Point> discretizeSpline(const GeometryTolerance& tolerance);
    };
}