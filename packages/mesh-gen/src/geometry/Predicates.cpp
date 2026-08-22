#include "geometry/Predicates.hpp"

#include <cmath>

namespace geometry {
    bool areCollinear(const std::vector<Point>& points, double distanceTolerance) {
        if (points.size() < 3) {
            return true;
        }

        const Point& a = points.front();
        const Point& b = points.back();

        const double dx = b.x - a.x;
        const double dy = b.y - a.y;

        const double length = std::hypot(dx, dy);

        if (length == 0.0) {
            return false;
        }

        for (auto point : points) {

            const double cross = dx * (point.y - a.y) - dy * (point.x - a.x);
            const double distance = std::abs(cross) / length;

            if (distance > distanceTolerance) {
                return false;
            }
        }

        return true;
    }

    constexpr double orientationSign(bool lexicographicallyGreater) noexcept {
        return lexicographicallyGreater ? 1.0 : -1.0;
    }

    bool lexographicGreater(const Point& p, const Point& q) {
        // p_{-2} is above all sites → lexicographically greatest
        if (p.type == PointType::MINUS2) {
            return q.type != PointType::MINUS2;
        }

        if (q.type == PointType::MINUS2) {
            return false;
        }

        // p_{-1} is below all sites → lexicographically least
        if (p.type == PointType::MINUS1) {
            return false;
        }

        if (q.type == PointType::MINUS1) {
            return true;
        }

        if (p.y != q.y) {
            return p.y > q.y;
        }
        return p.x < q.x;
    }

    double orient2dGeometric(const Point& a, const Point& b, const Point& c) {
        // Twice signed area of triangle abc
        return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    }

    double orient2dSymbolic(const Point& a, const Point& b, const Point& c) {

        const int mask =
            (a.isSymbolic() ? 1 : 0) | (b.isSymbolic() ? 2 : 0) | (c.isSymbolic() ? 4 : 0);

        switch (mask) {
        // c finite; a and/or b symbolic
        case 1: // a symbolic, b,c finite
            if (a.type == PointType::MINUS2) {
                return orientationSign(lexographicGreater(c, b)); // orient(M2, b, c)
            }
            // a == M1: orient(M1, b, c) = -orient(b, M1, c)
            return orientationSign(!lexographicGreater(c, b));

        case 2: // b symbolic, a,c finite
            if (b.type == PointType::MINUS1) {
                return orientationSign(lexographicGreater(c, a)); // orient(a, M1, c)
            }
            // b == M2: orient(a, M2, c) = -orient(M2, a, c)
            return orientationSign(!lexographicGreater(c, a));

        case 3: // a,b symbolic, c finite
            // orient(M2, M1, c) = +1, orient(M1, M2, c) = -1
            if (a.type == PointType::MINUS2 && b.type == PointType::MINUS1) {
                return 1.0;
            }
            if (a.type == PointType::MINUS1 && b.type == PointType::MINUS2) {
                return -1.0;
            }
            return 0.0;

        // c symbolic; reduce via orient(a,b,c) = orient(b,c,a)
        case 4: // c symbolic, a,b finite
            if (c.type == PointType::MINUS1) {
                // orient(a, b, M1) = orient(b, M1, a) = sign(a >_lex b)
                return orientationSign(lexographicGreater(a, b));
            }
            // c == M2: orient(a, b, M2) = orient(b, M2, a) = -orient(M2, b, a)
            //         = -sign(a >_lex b)
            return orientationSign(!lexographicGreater(a, b));

        case 5: // a,c symbolic, b finite
            // orient(a, b, c) = orient(b, c, a)
            if (a.type == PointType::MINUS2 && c.type == PointType::MINUS1) {
                // orient(M2, b, M1) = orient(b, M1, M2) = orient(M1, M2, b) = -1
                return -1.0;
            }
            if (a.type == PointType::MINUS1 && c.type == PointType::MINUS2) {
                // orient(M1, b, M2) = orient(b, M2, M1) = orient(M2, M1, b) = +1
                return 1.0;
            }
            return 0.0;

        case 6: // b,c symbolic, a finite
            if (b.type == PointType::MINUS1 && c.type == PointType::MINUS2) {
                // orient(a, M1, M2) = orient(M1, M2, a) = -1
                return -1.0;
            }
            if (b.type == PointType::MINUS2 && c.type == PointType::MINUS1) {
                // orient(a, M2, M1) = orient(M2, M1, a) = +1
                return 1.0;
            }
            return 0.0;

        default:
            return 0.0;
        }
    }

    double orient2D(const Point& a, const Point& b, const Point& c) {
        if (a.isSymbolic() || b.isSymbolic() || c.isSymbolic()) {
            return orient2dSymbolic(a, b, c);
        } else {
            return orient2dGeometric(a, b, c);
        }
    }

    double inCircleGeometric(const Point& a, const Point& b, const Point& c, const Point& d) {
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

    double inCircleSymbolic(const Point& a, const Point& b, const Point& c, const Point& d) {

        if (d.isSymbolic()) {
            return 0.0;
        }
        const int nSym =
            (a.isSymbolic() ? 1 : 0) + (b.isSymbolic() ? 1 : 0) + (c.isSymbolic() ? 1 : 0);

        // One symbolic vertex: circumcircle → half-plane.
        // For a CCW triangle, d is inside iff it lies left of the directed finite edge
        // opposite the symbolic vertex (same side as that vertex).
        if (nSym == 1) {
            if (c.isSymbolic()) {
                return orient2D(a, b, d);
            }
            if (a.isSymbolic()) {
                return orient2D(b, c, d);
            }
            return orient2D(c, a, d);
        }

        // Two symbolic vertices + one finite f.
        // Conflict with (f, p_{-1}, p_{-2}) iff f >_lex d, so the seed triangle on the
        // lex-max site conflicts with every other site, and later such triangles
        // follow the same lexicographic scheme.
        if (nSym == 2) {
            const Point& f = !a.isSymbolic() ? a : (!b.isSymbolic() ? b : c);
            return orientationSign(lexographicGreater(f, d));
        }

        return 0.0;
    }

    double inCircle(const Point& a, const Point& b, const Point& c, const Point& d) {

        if (a.isSymbolic() || b.isSymbolic() || c.isSymbolic() || d.isSymbolic()) {
            return inCircleSymbolic(a, b, c, d);
        }

        return inCircleGeometric(a, b, c, d);
    }
} // namespace geometry
