#include <gtest/gtest.h>

#include "geometry/GeometryUtils.hpp"
#include "geometry/Point.hpp"

using namespace geometry;

TEST(BoundingBox, ReturnsBoundingBox) {
    std::vector<Point> points;
    points.push_back(Point{2.0, 9.0});
    points.push_back(Point{0.0, 5.0});
    points.push_back(Point{1.0, 1.0});

    BoundingBox box;

    box = getboundingBox(points);

    EXPECT_DOUBLE_EQ(box.maxX, 2.0);
    EXPECT_DOUBLE_EQ(box.maxY, 9.0);
    EXPECT_DOUBLE_EQ(box.minX, 0.0);
    EXPECT_DOUBLE_EQ(box.minY, 1.0);
}
