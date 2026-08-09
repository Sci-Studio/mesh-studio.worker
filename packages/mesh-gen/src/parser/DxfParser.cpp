#include "parser/DxfParser.hpp"
#include "geometry/Point.hpp"

#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using namespace parser::dxf;

int DxfParser::addUnique(std::vector<Point>& points, const Point& point) {
    constexpr double eps = 1e-9;

    for (size_t i = 0; i < points.size(); ++i) {
        const Point& existing = points[i];
        if (std::abs(existing.x - point.x) <= eps &&
            std::abs(existing.y - point.y) <= eps) {
            return static_cast<int>(i);
        }
    }

    Point p = point;
    p.index = static_cast<int>(points.size());
    p.type = PointType::NORMAL;
    points.push_back(p);
    return p.index;
}

bool DxfParser::readPair(std::istream& inputFile, GROUP_CODE& code, std::string& value) {

    std::string line;
    std::istringstream istringstream;

    if(!std::getline(inputFile, line)) {
        return false;
    }
    istringstream.str(line);
    istringstream >> code;

    if(!std::getline(inputFile, line)) {
        return false;
    }
    istringstream.clear();
    istringstream.str(line);
    istringstream >> value;

    return true;
}

void DxfParser::parseLine(std::istream& inputFile, Mesh& mesh, GROUP_CODE& code, std::string& value) {

    Point start;
    Point end;
    start.x = start.y = 0.0;
    end.x = end.y = 0.0;

    while(readPair(inputFile, code, value) && code != 0) {
        if (code == dxf::START_X) {
            start.x = std::atof(value.c_str());
        } else if (code == dxf::START_Y) {
            start.y = std::atof(value.c_str());
        } else if (code == dxf::END_X) {
            end.x = std::atof(value.c_str());
        } else if (code == dxf::END_Y) {
            end.y = std::atof(value.c_str());
        }
    }

    const int i0 = addUnique(mesh.points, start);
    const int i1 = addUnique(mesh.points, end);
    if (i0 != i1) {
        mesh.constraints.push_back(Edge{i0, i1});
    }
}

void DxfParser::parseEntites(std::istream& inputFile, Mesh& mesh, GROUP_CODE& code, std::string& value) {

    while (code == dxf::ENTITY_TYPE && value != dxf::ENDSEC) {
        if (value == dxf::LINE) {
            parseLine(inputFile, mesh, code, value);
            continue;
        }
        // Skip unsupported entities until the next group-0 marker.
        while (readPair(inputFile, code, value) && code != dxf::ENTITY_TYPE) {
        }
    }
}

bool DxfParser::loadMesh(const char* path, Mesh& mesh) {
    mesh.points.clear();
    mesh.triangles.clear();
    mesh.constraints.clear();

    std::ifstream inputFile(path);
    if (!inputFile) {
      std::cerr << "Failed to open .dxf file: " << path << std::endl;
      return false;
    }

    std::string line;
    std::string value;
    GROUP_CODE code = 0;

    while(readPair(inputFile, code, value)) {

        if(code == dxf::NAME && value == dxf::ENTITIES) {
            readPair(inputFile, code, value);
            parseEntites(inputFile, mesh, code, value);
            break;
        }    
    }

    return true;
}