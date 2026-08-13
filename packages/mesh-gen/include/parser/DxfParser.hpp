#pragma once

#include "Parser.hpp"
#include "DxfCodes.hpp"
#include "geometry/Mesh.hpp"
#include "geometry/Point.hpp"

#include <istream>
#include <string>
#include <vector>

namespace parser {
    namespace dxf {

        class DxfParser : public Parser {
            public:
             bool loadMesh(const char* path, geometry::Mesh& mesh) override;
            
            private:
             int addUnique(std::vector<geometry::Point>& points, const geometry::Point& point);
             bool readPair(std::istream& inputFile, GROUP_CODE& code, std::string& value);
            
             void addConstraintEdge(geometry::Mesh& mesh, int i0, int i1);
             void addPolylineConstraints(geometry::Mesh& mesh,
                                         const std::vector<geometry::Point>& poly);
            
             void parseLine(std::istream& inputFile, geometry::Mesh& mesh, GROUP_CODE& code, std::string& value);
             void parseArc(std::istream& inputFile, geometry::Mesh& mesh, GROUP_CODE& code, std::string& value);
             void parseSpline(std::istream& inputFile, geometry::Mesh& mesh, GROUP_CODE& code, std::string& value);
             void parseEntites(std::istream& inputFile, geometry::Mesh& mesh, GROUP_CODE& code, std::string& value);
        };

    }  
}  