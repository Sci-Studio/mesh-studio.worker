#pragma once

#include "DataStructures.hpp"

#include <utility>
#include <vector>

struct BoundingBox {
    double min_x, max_x;
    double min_y, max_y;
};

BoundingBox getboundingBox(const std::vector<Vec2>& points);
