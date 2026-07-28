#include "delaunay.h"
#include "data_structures.h"

#include <iostream>

Triangle make_tri(int a, int b, int c) {
    Triangle t;
    t.v[0] = a;
    t.v[1] = b;
    t.v[2] = c;

    t.neigh[0] = -1;
    t.neigh[1] = -1;
    t.neigh[2] = -1;
    return t;
}

typedef std::pair<int, int> EdgeKey;

EdgeKey undirected_edge(int a, int b) {
    return (a < b) ? EdgeKey{a, b} : EdgeKey{b, a};
}

bool triangulate(Mesh& mesh) {
    mesh.triangles.clear();

    const int n = static_cast<int>(mesh.points.size());
    if (n < 3) {
        return false;
    }

    double min_x = mesh.points[0].x;
    double min_y = mesh.points[0].y;
    double max_x = min_x;
    double max_y = min_y;

    for (int i = 1; i < n; ++i) {
        const Vec2& p = mesh.points[i];
        if (p.x < min_x) min_x = p.x;
        if (p.y < min_y) min_y = p.y;
        if (p.x > max_x) max_x = p.x;
        if (p.y > max_y) max_y = p.y;
    }

    const double dx = max_x - min_x;
    const double dy = max_y - min_y;
    const double delta = std::max(dx, dy);
    const double mid_x = 0.5 * (min_x + max_x);
    const double mid_y = 0.5 * (min_y + max_y);

    const double m = (delta > 0.0) ? (20.0 * delta) : 20.0;
    
    const int s0 = n;
    const int s1 = n + 1;
    const int s2 = n + 2;

    mesh.points.push_back(Vec2{mid_x - 2.0 * m, mid_y -m});
    mesh.points.push_back(Vec2{mid_x, mid_y + 2.0 * m});
    mesh.points.push_back(Vec2{mid_x + 2.0 * m, mid_y - m});

    // ensure super-triangle is CCW (counter clockwise)
    if(orient2d(mesh.points[s0], mesh.points[s1], mesh.points[s2]) > 0.0){
        mesh.triangles.push_back(make_tri(s0, s1, s2));
    } else {
        mesh.triangles.push_back(make_tri(s0, s2, s1));
    }

    for (int pi = 0; pi < n; ++pi) {
        const Vec2& p = mesh.points[pi];

        std::vector<int> bad;
        bad.reserve(mesh.triangles.size());

        for (int ti = 0; ti < static_cast<int>mesh.triangles.size(), ++i) {
            const Triangle& t = mesh.triangles[ti];
            const Vec2& a = mesh.points[t.v[0]];
            const Vec2& b = mesh.points[t.v[1]];
            const Vec2& c = mesh.points[t.v[2]];

            if (incircle(a, b, c, p) > 0.0) {
                bad.push_back(ti);
            }
        }

        // Count undirected edges among bad triangles; boundary = count == 1
        std::map<EdgeKey, int> edge_count;
        std::map<EdgeKey, EdgeKey> directed;  // store one directed (a->b) for orientation
       
        for (size_t bi = 0; bi < bad.size(); ++bi) {
            const Triangle& t = mesh.triangles[bad[bi]];
            const int verts[3] = {t.v[0], t.v[1], t.v[2]};
            for (int e = 0; e < 3; ++e) {
                const int a = verts[e];
                const int b = verts[(e + 1) % 3];
                const EdgeKey key = undirected_edge(a, b);
                edge_count[key] += 1;
                directed[key] = EdgeKey(a, b);  // CCW edge from the bad triangle
            }
        }

    }

    return true;
}