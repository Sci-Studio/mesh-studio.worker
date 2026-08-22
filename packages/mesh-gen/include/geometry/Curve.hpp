#pragma once

#include "Point.hpp"

#include <vector>

namespace geometry {

    struct DiscretizationOptions {
        double maxChordError = 0.01;
        double maxAngleDegree = 10.0;
        double pointMerge = 1e-8;
    };

    class Curve {
      public:
        virtual ~Curve() = default;

        virtual std::vector<Point> discretize(const DiscretizationOptions& options) const = 0;
        virtual bool isClosed() const = 0;
    };

} // namespace geometry
