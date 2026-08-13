#pragma once

#include "Parser.hpp"
#include "geometry/Mesh.hpp"

namespace parser {
  namespace off {

    class OffParser : public Parser {
      public:
        bool loadMesh(const char* path, geometry::Mesh& mesh) override;
    };

  }
}
