#include "parser/DxfParser.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using namespace parser::dxf;

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

    Vec2 start;
    Vec2 end;
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

    addUnique(mesh.points, start);
    addUnique(mesh.points, end);

}

void DxfParser::parseEntites(std::istream& inputFile, Mesh& mesh, GROUP_CODE& code, std::string& value) {

    while(code == dxf::ENTITY_TYPE && value != dxf::ENDSEC) {
        if (code == dxf::ENTITY_TYPE && value == dxf::LINE) {
            parseLine(inputFile, mesh, code, value);
            continue;
        }
    }
}

bool DxfParser::loadMesh(const char* path, Mesh& mesh) {
    mesh.points.clear();
    mesh.triangles.clear();

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