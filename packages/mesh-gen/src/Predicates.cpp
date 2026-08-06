#include "Predicates.hpp"

double orient2d(const Point& a, const Point& b, const Point& c) {
    // Twice signed area of triangle abc
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

double incircle(const Point& a, const Point& b, const Point& c, const Point& d) {
    // Determinant form: >0 if d is inside circle of CCW abc
    const double adx = a.x - d.x;
    const double ady = a.y - d.y;
    const double bdx = b.x - d.x;
    const double bdy = b.y - d.y;
    const double cdx = c.x - d.x;
    const double cdy = c.y - d.y;
    
    const double abdet = adx * bdy - bdx * ady;
    const double bcdet = bdx * cdy - cdx * bdy;
    const double cadet = cdx * ady - adx * cdy;
    const double alift = adx * adx + ady * ady;
    const double blift = bdx * bdx + bdy * bdy;
    const double clift = cdx * cdx + cdy * cdy;
    
    return alift * bcdet + blift * cadet + clift * abdet;
}