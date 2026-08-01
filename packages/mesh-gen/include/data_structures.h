#pragma once

#include <vector>

const unsigned int COMPLEX_NO_VERTICES = 3;

struct Vec2 {
    double x, y;
};

struct Triangle {
    int v[COMPLEX_NO_VERTICES];
    int neigh[COMPLEX_NO_VERTICES];
};

struct Mesh {
    std::vector<Vec2> points;
    std::vector<Triangle> triangles;
};