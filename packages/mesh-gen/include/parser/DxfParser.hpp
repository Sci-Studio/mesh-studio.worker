#pragma once

#include "DxfCodes.hpp"
#include "Parser.hpp"
#include "geometry/Mesh.hpp"

#include <istream>
#include <string>

namespace parser {
    namespace dxf {

        class DxfParser : public Parser {
          public:
            bool loadMesh(const char* path, geometry::Mesh& mesh) override;

          private:
            bool readPair(std::istream& inputFile, GROUP_CODE& code, std::string& value);
            void parseLine(std::istream& inputFile, geometry::Mesh& mesh, GROUP_CODE& code,
                           std::string& value);
            void parseArc(std::istream& inputFile, geometry::Mesh& mesh, GROUP_CODE& code,
                          std::string& value);
            void parseCircle(std::istream& inputFile, geometry::Mesh& mesh, GROUP_CODE& code,
                             std::string& value);
            void parseSpline(std::istream& inputFile, geometry::Mesh& mesh, GROUP_CODE& code,
                             std::string& value);
            void parseEntites(std::istream& inputFile, geometry::Mesh& mesh, GROUP_CODE& code,
                              std::string& value);
        };

    } // namespace dxf
} // namespace parser
