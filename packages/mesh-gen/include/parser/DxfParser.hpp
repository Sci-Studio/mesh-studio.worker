#pragma once

#include "DataStructures.hpp"
#include "DxfCodes.hpp"

#include <fstream>

namespace dxf {

    class DxfParser {

        public:
            bool loadDxf(const char* path, Mesh& mesh);
        
        private:
            bool addUnique(std::vector<Vec2>& points, const Vec2& point);
            bool readPair(std::istream& inputFile, dxf::GROUP_CODE& code, std::string& value);
            void parseLine(std::istream& inputFile, Mesh& mesh, dxf::GROUP_CODE& code, std::string& value);
            void parseEntites(std::istream& inputFile, Mesh& mesh, dxf::GROUP_CODE& code, std::string& value);
    };
}
