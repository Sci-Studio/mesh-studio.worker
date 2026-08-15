#pragma once

#include "Curve.hpp"
#include "Point.hpp"


namespace geometry {

  class Spline : public Curve {
    public:
      int degree = 0;
      std::vector<Point> controlPoints;
      std::vector<double> knots;
      std::vector<double> weights;
    
      std::vector<Point> discretize(const DiscretizationOptions& options) const override;
      bool isClosed() const override;
  };

}
