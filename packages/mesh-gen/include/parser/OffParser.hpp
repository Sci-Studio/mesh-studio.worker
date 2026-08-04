#pragma once

#include "Parser.hpp"
#include "DataStructures.hpp"

namespace parser {
    namespace off {
    
        class OffParser: public Parser {
            
            public:
                bool loadMesh(const char* path, Mesh& mesh) override;
        };
    }
}