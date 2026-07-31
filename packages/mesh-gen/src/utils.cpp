#include "utils.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

typedef std::vector<Vec2>::const_iterator PointIt;

BBox bounding_box(const std::vector<Vec2>& points) {

    if (points.empty()) {
        throw std::invalid_argument("Point vector is empty");
    }

    BBox box {
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

std::pair<double, double> point_minmax(const std::vector<Vec2>& points) {
    std::pair<PointIt, PointIt> mmx = std::minmax_element(points.begin(), points.end(),
            [](const Vec2& a, const Vec2& b) {return a.x < b.x;});
    return std::make_pair(mmx.first->x, mmx.second->x);         
}