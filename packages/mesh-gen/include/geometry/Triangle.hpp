#pragma once

namespace geometry {

  class Triangle {
    public:
     static constexpr unsigned int kVertexCount = 3;
    
     int v[kVertexCount]{};
     int neigh[kVertexCount]{};
    
     Triangle() {
        for (unsigned int i = 0; i < kVertexCount; ++i) {
          v[i] = -1;
          neigh[i] = -1;
        }
     }
   
     Triangle(int a, int b, int c) {
        v[0] = a;
        v[1] = b;
        v[2] = c;
        neigh[0] = -1;
        neigh[1] = -1;
        neigh[2] = -1;
     }
  };

}
