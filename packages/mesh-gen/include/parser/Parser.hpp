#pragma once

#include "DataStructures.hpp"

namespace parser {
    
    class Parser {

        public:
            virtual ~Parser() = default;
            virtual bool loadMesh(const char* path, Mesh& mesh) = 0;
    };
}