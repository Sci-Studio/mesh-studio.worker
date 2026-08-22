#pragma once

namespace geometry {

    class Edge {
      public:
        int a = -1;
        int b = -1;

        Edge() = default;
        Edge(int a_, int b_) : a(a_), b(b_) {}
    };

} // namespace geometry
