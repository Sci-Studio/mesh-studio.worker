#pragma once

namespace geometry {

    class Triangle {
      public:
        static constexpr unsigned int kVertexCount = 3;

        int vertices[kVertexCount]{};
        int neighbour[kVertexCount]{};

        Triangle() {
            for (unsigned int i = 0; i < kVertexCount; ++i) {
                vertices[i] = -1;
                neighbour[i] = -1;
            }
        }

        Triangle(int a, int b, int c) {
            vertices[0] = a;
            vertices[1] = b;
            vertices[2] = c;
            neighbour[0] = -1;
            neighbour[1] = -1;
            neighbour[2] = -1;
        }
    };

} // namespace geometry
