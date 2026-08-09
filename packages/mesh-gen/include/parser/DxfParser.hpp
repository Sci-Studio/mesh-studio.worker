#pragma once

#include "Parser.hpp"
#include "DataStructures.hpp"
#include "DxfCodes.hpp"
#include "geometry/Point.hpp"

#include <istream>
#include <vector>

namespace parser {
    namespace dxf {

        class DxfParser : public Parser {

            public:
                bool loadMesh(const char* path, Mesh& mesh) override;

            private:
                // Inserts point if not already present (eps). Returns index in points.
                int addUnique(std::vector<Point>& points, const Point& point);
                bool readPair(std::istream& inputFile, dxf::GROUP_CODE& code, std::string& value);

                void addConstraintEdge(Mesh& mesh, int i0, int i1);
                void addPolylineConstraints(Mesh& mesh, const std::vector<Point>& poly);

                void parseLine(std::istream& inputFile, Mesh& mesh, GROUP_CODE& code, std::string& value);
                void parseArc(std::istream& inputFile, Mesh& mesh, GROUP_CODE& code, std::string& value);
                void parseSpline(std::istream& inputFile, Mesh& mesh, GROUP_CODE& code, std::string& value);
                void parseEntites(std::istream& inputFile, Mesh& mesh, GROUP_CODE& code, std::string& value);
        };
    }
}
