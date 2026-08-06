#pragma once

#include "Parser.hpp"
#include "DataStructures.hpp"
#include "DxfCodes.hpp"
#include "geometry/Point.hpp"

#include <istream>

namespace parser {
    namespace dxf {

        class DxfParser : public Parser {
    
            public:
                bool loadMesh(const char* path, Mesh& mesh) override;
            
            private:
                bool addUnique(std::vector<Point>& points, const Point& point);
                bool readPair(std::istream& inputFile, dxf::GROUP_CODE& code, std::string& value);
                void parseLine(std::istream& inputFile, Mesh& mesh, dxf::GROUP_CODE& code, std::string& value);
                void parseEntites(std::istream& inputFile, Mesh& mesh, dxf::GROUP_CODE& code, std::string& value);
        };
    }
}