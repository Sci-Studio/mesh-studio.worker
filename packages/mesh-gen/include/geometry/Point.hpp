#pragma once

namespace geometry {
    
    enum class PointType {
        NORMAL,
        MINUS1,
        MINUS2
    };

    class Point {
        public:
            double x;
            double y;
            int index;
    };
}