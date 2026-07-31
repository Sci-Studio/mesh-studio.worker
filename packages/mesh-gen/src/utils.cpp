#include "utils.h"

#include <algorithm>
#include <utility>

typedef std::vector<Vec2>::const_iterator PointIt;

std::pair<double, double> point_minmax(const std::vector<Vec2>& points) {
    std::pair<PointIt, PointIt> mmx = std::minmax_element(points.begin(), points.end(),
            [](const Vec2& a, const Vec2& b) {return a.x < b.x;});
    return std::make_pair(mmx.first->x, mmx.second->x);         
}