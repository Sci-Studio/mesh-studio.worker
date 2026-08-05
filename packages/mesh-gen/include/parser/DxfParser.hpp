#pragma once

#include "Parser.hpp"
#include "DataStructures.hpp"
#include "DxfCodes.hpp"

#include <istream>

namespace parser {
    namespace dxf {

        class DxfParser : public Parser {
    
            public:
                bool loadMesh(const char* path, Mesh& mesh) override;
            
            private:
                bool addUnique(std::vector<Vec2>& points, const Vec2& point);
                bool readPair(std::istream& inputFile, dxf::GROUP_CODE& code, std::string& value);
                void parseLine(std::istream& inputFile, Mesh& mesh, dxf::GROUP_CODE& code, std::string& value);
                void parseEntites(std::istream& inputFile, Mesh& mesh, dxf::GROUP_CODE& code, std::string& value);
        };
    }
}