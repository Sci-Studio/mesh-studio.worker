#pragma once

#include "data_structures.h"

#include <utility>
#include <vector>

struct BBox {
    double min_x, max_x;
    double min_y, max_y;
};

BBox bounding_box(const std::vector<Vec2>& points);

std::pair<double, double> point_minmax(const std::vector<Vec2>& points);