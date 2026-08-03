#include <gtest/gtest.h>
#include "Utils.hpp"

TEST(PointMinMax, ReturnsMinAndMaxX) {
    std::vector<Vec2> points;
    points.push_back(Vec2{2.0, 9.0});
    points.push_back(Vec2{0.0, 5.0});
    points.push_back(Vec2{1.0, 1.0});
    
    std::pair<double, double> mm = point_minmax(points);
    
    EXPECT_DOUBLE_EQ(mm.first, 0.0);
    EXPECT_DOUBLE_EQ(mm.second, 2.0);
}

TEST(BoundingBox, ReturnsBoundingBox) {
    std::vector<Vec2> points;
    points.push_back(Vec2{2.0, 9.0});
    points.push_back(Vec2{0.0, 5.0});
    points.push_back(Vec2{1.0, 1.0});

    BBox box;

    box = bounding_box(points);

    EXPECT_DOUBLE_EQ(box.max_x, 2.0);
    EXPECT_DOUBLE_EQ(box.max_y, 9.0);
    EXPECT_DOUBLE_EQ(box.min_x, 0.0);
    EXPECT_DOUBLE_EQ(box.min_y, 1.0);
}