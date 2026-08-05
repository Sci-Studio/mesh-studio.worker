#pragma once

#include "DataStructures.hpp"

#include <vector>

struct BoundingBox {
    double minX, maxX;
    double minY, maxY;
};

BoundingBox getboundingBox(const std::vector<Vec2>& points);
