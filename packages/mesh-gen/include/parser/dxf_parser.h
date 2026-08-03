#pragma once

#include "../data_structures.h"
#include "dxf_codes.h"

#include <fstream>

namespace DXF {

    class DxfParser {

        public:
            bool loadDxf(const char* path, Mesh& mesh);
        
        private:
            bool addUnique(std::vector<Vec2>& points, const Vec2& point);
            bool readPair(std::istream& inputFile, DXF::GROUP_CODE& code, std::string& value);
            void parseLine(std::istream& inputFile, Mesh& mesh, DXF::GROUP_CODE& code, std::string& value);
            void parseEntites(std::istream& inputFile, Mesh& mesh, DXF::GROUP_CODE& code, std::string& value);
    };
}
