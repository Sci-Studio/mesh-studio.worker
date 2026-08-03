#include "Utils.hpp"

#include <algorithm>
#include <stdexcept>
#include <utility>

BoundingBox getboundingBox(const std::vector<Vec2>& points) {

    if (points.empty()) {
        throw std::invalid_argument("Point vector is empty");
    }

    BoundingBox box {
        points[0].x, points[0].x,
        points[0].y, points[0].y
    };

    for (const auto& p : points) {
        box.min_x = std::min(box.min_x, p.x);
        box.max_x = std::max(box.max_x, p.x);

        box.min_y = std::min(box.min_y, p.y);
        box.max_y = std::max(box.max_y, p.x);

    }
    return box;
}
