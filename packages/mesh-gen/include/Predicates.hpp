#pragma once

#include "geometry/Point.hpp"

using namespace geometry;

bool lexographicGreater(const Point& a, const Point& b);

double orient2D(const Point& a, const Point& b, const Point& c);

double inCircle(const Point& a, const Point& b, const Point& c, const Point& d);