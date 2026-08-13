#include "Delaunay.hpp"
#include "geometry/Mesh.hpp"
#include "geometry/Predicates.hpp"
#include "geometry/Point.hpp"

#include <map>
#include <algorithm>
#include <vector>

using namespace geometry;

using EdgeKey = std::pair<int, int>;

EdgeKey undirectedEdge(int a, int b) {
    return (a < b) ? EdgeKey{a, b} : EdgeKey{b, a};
}

bool triangulate(Mesh& mesh) {
    mesh.triangles.clear();

    const int n = static_cast<int>(mesh.points.size());
    if (n < static_cast<int>(Triangle::kVertexCount)) {
        return false;
    }

    // Lex-max site p0 (highest y, then smallest x) seeds the symbolic super-triangle
    // with p_{-1} and p_{-2} (no large finite coordinates).
    int p0 = 0;
    for (int i = 1; i < n; ++i) {
        if (lexographicGreater(mesh.points[i], mesh.points[p0])) {
            p0 = i;
        }
    }

    const int m1 = n;      // array index of p_{-1}
    const int m2 = n + 1;  // array index of p_{-2}
    mesh.points.emplace_back(Point{0.0, 0.0, PointType::MINUS1, -1});
    mesh.points.emplace_back(Point{0.0, 0.0, PointType::MINUS2, -2});

    // orient(p0, p_{-2}, p_{-1}) > 0 → CCW seed triangle
    mesh.triangles.push_back(Triangle(p0, m2, m1));

    for (int pi = 0; pi < n; ++pi) {
        if (pi == p0) {
            continue;  // already a vertex of the seed triangle
        }

        const Point& p = mesh.points[pi];

        std::vector<int> bad;
        bad.reserve(mesh.triangles.size());

        for (int ti = 0; ti < static_cast<int>(mesh.triangles.size()); ++ti) {
            const Triangle& t = mesh.triangles[ti];
            const Point& a = mesh.points[t.vertices[0]];
            const Point& b = mesh.points[t.vertices[1]];
            const Point& c = mesh.points[t.vertices[2]];
            if (inCircle(a, b, c, p) > 0.0) {
                bad.push_back(ti);
            }
        }

        // Count undirected edges among bad triangles; boundary = count == 1
        std::map<EdgeKey, int> edgeCount;
        std::map<EdgeKey, EdgeKey> directed;

        for (size_t bi = 0; bi < bad.size(); ++bi) {
            const Triangle& t = mesh.triangles[bad[bi]];
            const int verts[Triangle::kVertexCount] = { t.vertices[0], t.vertices[1], t.vertices[2] };
            for (unsigned int e = 0; e < Triangle::kVertexCount; ++e) {
                const int a = verts[e];
                const int b = verts[(e + 1) % Triangle::kVertexCount];
                const EdgeKey key = undirectedEdge(a, b);
                edgeCount[key] += 1;
                directed[key] = EdgeKey(a, b);
            }
        }

        std::vector<EdgeKey> boundary;
        for (const auto& entry : edgeCount) {
            if (entry.second == 1) {
                boundary.push_back(directed[entry.first]);
            }
        }

        std::sort(bad.begin(), bad.end());
        for (int i = static_cast<int>(bad.size()) - 1; i >= 0; --i) {
            mesh.triangles.erase(mesh.triangles.begin() + bad[i]);
        }

        for (size_t ei = 0; ei < boundary.size(); ++ei) {
            int a = boundary[ei].first;
            int b = boundary[ei].second;
            if (orient2D(mesh.points[a], mesh.points[b], p) <= 0.0) {
                const int tmp = a;
                a = b;
                b = tmp;
            }
            mesh.triangles.push_back(Triangle(a, b, pi));
        }
    }

    // Drop any triangle that still touches a symbolic vertex
    {
        std::vector<Triangle> kept;
        kept.reserve(mesh.triangles.size());
        for (size_t i = 0; i < mesh.triangles.size(); ++i) {
            const Triangle& t = mesh.triangles[i];
            if (t.vertices[0] >= n || t.vertices[1] >= n || t.vertices[2] >= n) {
                continue;
            }
            kept.push_back(t);
        }
        mesh.triangles.swap(kept);
    }

    mesh.points.resize(n);

    return !mesh.triangles.empty();
}
