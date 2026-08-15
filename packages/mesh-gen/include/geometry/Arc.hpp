#pragma once

#include "Curve.hpp"
#include "Point.hpp"

namespace geometry {

  class Arc : public Curve {
    public:
      Point center{};
      double radius = 0.0;
      double startAngleDegree = 0.0;
      double endAngleDegree = 0.0;
    
      Arc() = default;
      Arc(const Point& center_, double radius_, double startAngleDegree_,
          double endAngleDegree_);
      
      std::vector<Point> discretize(const DiscretizationOptions& options) const override;
      bool isClosed() const override;
  };

}
