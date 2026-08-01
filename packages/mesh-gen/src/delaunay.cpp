#include "delaunay.h"
#include "data_structures.h"
#include "predicates.h"
#include "utils.h"

#include <map>
#include <iostream>
#include <algorithm>

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
    if (n < COMPLEX_NO_VERTICES) {
        return false;
    }

    BBox box = bounding_box(mesh.points);

    const double dx = box.max_x - box.min_x;
    const double dy = box.max_y - box.min_y;
    const double delta = std::max(dx, dy);
    const double mid_x = 0.5 * (box.min_x + box.max_x);
    const double mid_y = 0.5 * (box.min_y + box.max_y);

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

        for (int ti = 0; ti < static_cast<int>(mesh.triangles.size()); ++ti) {
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
            const int verts[COMPLEX_NO_VERTICES] = {t.v[0], t.v[1], t.v[2]};
            for (int e = 0; e < COMPLEX_NO_VERTICES; ++e) {
                const int a = verts[e];
                const int b = verts[(e + 1) % COMPLEX_NO_VERTICES];
                const EdgeKey key = undirected_edge(a, b);
                edge_count[key] += 1;
                directed[key] = EdgeKey(a, b);  // CCW edge from the bad triangle
            }
        }

        std::vector<EdgeKey> boundary;
        for(std::map<EdgeKey, int>::const_iterator it = edge_count.begin(); it != edge_count.end(); ++it) {
            if (it->second == 1) {
                boundary.push_back(directed[it->first]);
            }
        }

        std::sort(bad.begin(), bad.end());
        for(int i = static_cast<int>(bad.size()) - 1; i >= 0; --i) {
            mesh.triangles.erase(mesh.triangles.begin() + bad[i]);
        }

        // connect p to each boundary edge
      
        for (size_t ei = 0; ei < boundary.size(); ++ei) {
            int a = boundary[ei].first;
            int b = boundary[ei].second;
            // Ensure (a,b,p) is CCW
            if (orient2d(mesh.points[a], mesh.points[b], p) <= 0.0) {
              const int tmp = a;
              a = b;
              b = tmp;
            }
            mesh.triangles.push_back(make_tri(a, b, pi));
        }
    }

     // Remove triangles that touch any super-triangle vertex
    {
      std::vector<Triangle> kept;
      kept.reserve(mesh.triangles.size());
      for (size_t i = 0; i < mesh.triangles.size(); ++i) {
        const Triangle& t = mesh.triangles[i];
        if (t.v[0] >= n || t.v[1] >= n || t.v[2] >= n) {
          continue;
        }
        kept.push_back(t);
      }
      mesh.triangles.swap(kept);
    }

    // Drop super-triangle vertices (indices >= n are unused)
    mesh.points.resize(n);

    return !mesh.triangles.empty();
}