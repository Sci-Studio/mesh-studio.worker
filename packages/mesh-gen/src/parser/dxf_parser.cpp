#include "parser/dxf_parser.h"
#include "parser/dxf_codes.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>

using namespace DXF;

bool DxfParser::addUnique(std::vector<Vec2>& points, const Vec2& point) {

    constexpr double eps = 1e-9;

    bool contains = std::ranges::any_of(points, [&](const Vec2& existing) {
        return std::abs(existing.x - point.x) <= eps &&
               std::abs(existing.y - point.y) <= eps;
    });

    if (!contains) {
        points.push_back(point);
        return true;
    }

    return false;
}

bool DxfParser::readPair(std::istream& inputFile, GROUP_CODE& code, std::string& value) {

    std::string line;
    std::istringstream iss;

    if(!std::getline(inputFile, line)) {
        return false;
    }
    iss.str(line);
    iss >> code;

    if(!std::getline(inputFile, line)) {
        return false;
    }
    iss.clear();
    iss.str(line);
    iss >> value;

    return true;
}

void DxfParser::parseLine(std::istream& inputFile, Mesh& mesh, GROUP_CODE& code, std::string& value) {

    Vec2 start;
    Vec2 end;
    start.x = start.y = 0.0;
    end.x = end.y = 0.0;
    
    while(readPair(inputFile, code, value) && code != 0) {
        if (code == DXF::START_X) {
            start.x = std::atof(value.c_str());
        } else if (code == DXF::START_Y) {
            start.y = std::atof(value.c_str());
        } else if (code == DXF::END_X) {
            end.x = std::atof(value.c_str());
        } else if (code == DXF::END_Y) {
            end.y = std::atof(value.c_str());
        }
    }

    addUnique(mesh.points, start);
    addUnique(mesh.points, end);

}

void DxfParser::parseEntites(std::istream& in, Mesh& mesh, GROUP_CODE& code, std::string& value) {

    while(code == DXF::ENTITY_TYPE && value != DXF::ENDSEC) {
        if (code == DXF::ENTITY_TYPE && value == DXF::LINE) {
            parseLine(in, mesh, code, value);
            continue;
        }
    }
}

bool DxfParser::loadDxf(const char* path, Mesh& mesh) {
    mesh.points.clear();
    mesh.triangles.clear();

    std::ifstream in(path);
    if (!in) {
      std::cerr << "Failed to open .dxf file: " << path << std::endl;
      return false;
    }

    std::string line;
    std::string value;
    std::istringstream iss;
    GROUP_CODE code = 0;


    while(readPair(in, code, value)) {

        if(code == DXF::NAME && value == DXF::ENTITIES) {
            readPair(in, code, value);
            parseEntites(in, mesh, code, value);
            break;
        }

        
    }

    return true;
}