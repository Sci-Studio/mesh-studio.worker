#pragma once

#include "geometry/Point.hpp"

using namespace geometry;

double orient2d(const Point& a, const Point& b, const Point& c);

double incircle(const Point& a, const Point& b, const Point& c, const Point& d);