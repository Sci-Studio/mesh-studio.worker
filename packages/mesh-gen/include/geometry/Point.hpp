#pragma once

namespace geometry {

    enum class PointType { NORMAL, MINUS1, MINUS2 };

    class Point {
      public:
        double x;
        double y;
        PointType type = PointType::NORMAL;
        int index;

        bool isSymbolic() const {
            return type == PointType::MINUS1 || type == PointType::MINUS2;
        }
    };
} // namespace geometry
