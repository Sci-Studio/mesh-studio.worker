#pragma once

#include "./geometry/Point.hpp"


#include <vector>

using namespace geometry;

const unsigned int COMPLEX_NO_VERTICES = 3;

struct Triangle {
    int v[COMPLEX_NO_VERTICES];
    int neigh[COMPLEX_NO_VERTICES];
};

struct Mesh {
    std::vector<Point> points;
    std::vector<Triangle> triangles;
};