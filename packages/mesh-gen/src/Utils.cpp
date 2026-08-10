#include "Utils.hpp"

#include <algorithm>
#include <stdexcept>

BoundingBox getboundingBox(const std::vector<Point>& points) {

    if (points.empty()) {
        throw std::invalid_argument("Point vector is empty");
    }

    BoundingBox box {
        points[0].x, points[0].x,
        points[0].y, points[0].y
    };

    for (const auto& p : points) {
        box.minX = std::min(box.minX, p.x);
        box.maxX = std::max(box.maxX, p.x);

        box.minY = std::min(box.minY, p.y);
        box.maxY = std::max(box.maxY, p.y);

    }
    return box;
}
