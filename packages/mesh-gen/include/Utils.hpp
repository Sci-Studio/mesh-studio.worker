#pragma once

#include "geometry/Point.hpp"

#include <vector>

using namespace geometry;

struct BoundingBox {
    double minX, maxX;
    double minY, maxY;
};

BoundingBox getboundingBox(const std::vector<Point>& points);
