#pragma once

#include "geometry/Mesh.hpp"

namespace parser {

    class Parser {
      public:
        virtual ~Parser() = default;
        virtual bool loadMesh(const char* path, geometry::Mesh& mesh) = 0;
    };

} // namespace parser
