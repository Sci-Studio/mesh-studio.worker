#pragma once

#include <vector>

struct Vec2 {
    double x, y;
};

struct Triangle {
    int v[3];
    int neigh[3];
};

struct Mesh {
    std::vector<Vec2> points;
    std::vector<Triangle> triangles;
};