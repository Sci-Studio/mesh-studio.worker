#include <gtest/gtest.h>
#include <algorithm>
#include <iostream>
#include "data_structures.h"
#include "parser/dxf_parser.h"

TEST(ParseDFXFile, ReturnsPointsInDfx) {
    Mesh mesh;
    DXF::DxfParser parser;

    const char* dfx_path = "../data/rectangle-prism-Shape2DView.dxf";
    
    bool is_loaded = parser.loadDxf(dfx_path, mesh);

    ASSERT_TRUE(is_loaded);
    ASSERT_FALSE(mesh.points.empty());
    Vec2 p0 = {5.0, -2.5};
    Vec2 p1 = {5.0, 2.5};
    
    constexpr double eps = 1e-9;

    auto p0_it = std::ranges::find_if(
      mesh.points,
      [&](const Vec2& p)
      {
          return std::abs(p0.x - p.x) < eps &&
                 std::abs(p0.y - p.y) < eps;
      });

    auto p1_it = std::ranges::find_if(
      mesh.points,
      [&](const Vec2& p)
      {
          return std::abs(p1.x - p.x) < eps &&
                 std::abs(p1.y - p.y) < eps;
      });

    
    EXPECT_NE(p0_it, mesh.points.end());
    EXPECT_NE(p1_it, mesh.points.end());

}
