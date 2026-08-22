#pragma once

#include "Curve.hpp"
#include "Point.hpp"

namespace geometry {

    class Circle : public Curve {
      public:
        Point center;
        double radius = 0.0;

        Circle() = default;
        Circle(const Point& center_, double radius_);

        std::vector<Point> discretize(const DiscretizationOptions& options) const override;
        bool isClosed() const override;
    };

} // namespace geometry
