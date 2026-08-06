#include <gtest/gtest.h>
#include <algorithm>
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
