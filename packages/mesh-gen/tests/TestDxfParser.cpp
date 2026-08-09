#include <gtest/gtest.h>
#include <algorithm>
#include <cmath>
#include "DataStructures.hpp"
#include "parser/DxfParser.hpp"
#include "geometry/Point.hpp"

TEST(ParseDFXFile, ReturnsPointsInDfx) {
    Mesh mesh;
    parser::dxf::DxfParser parser;

    const char* dfxPath = "../data/rectangle-prism-Shape2DView.dxf";

    bool isLoaded = parser.loadMesh(dfxPath, mesh);

    ASSERT_TRUE(isLoaded);
    ASSERT_FALSE(mesh.points.empty());
    Point p0 = {5.0, -2.5};
    Point p1 = {5.0, 2.5};

    constexpr double eps = 1e-9;

    auto p0It = std::ranges::find_if(
      mesh.points,
      [&](const Point& p)
      {
          return std::abs(p0.x - p.x) < eps &&
                 std::abs(p0.y - p.y) < eps;
      });

    auto p1It = std::ranges::find_if(
      mesh.points,
      [&](const Point& p)
      {
          return std::abs(p1.x - p.x) < eps &&
                 std::abs(p1.y - p.y) < eps;
      });

    EXPECT_NE(p0It, mesh.points.end());
    EXPECT_NE(p1It, mesh.points.end());
}

TEST(ParseDFXFile, LineEntitiesPopulateConstraintEdges) {
    Mesh mesh;
    parser::dxf::DxfParser parser;

    ASSERT_TRUE(parser.loadMesh("../data/rectangle-prism-Shape2DView.dxf", mesh));

    // Axis-aligned rectangle: 4 unique corners, 4 LINE constraints
    EXPECT_EQ(mesh.points.size(), 4u);
    EXPECT_EQ(mesh.constraints.size(), 4u);

    const int n = static_cast<int>(mesh.points.size());
    for (const Edge& e : mesh.constraints) {
        EXPECT_GE(e.a, 0);
        EXPECT_GE(e.b, 0);
        EXPECT_LT(e.a, n);
        EXPECT_LT(e.b, n);
        EXPECT_NE(e.a, e.b);
    }
}
