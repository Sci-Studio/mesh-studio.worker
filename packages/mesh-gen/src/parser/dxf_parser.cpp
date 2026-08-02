#include "parser/dxf_parser.h"
#include "parser/dxf_codes.h"

#include <fstream>
#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>

bool add_unique(std::vector<Vec2>& points, const Vec2& p) {

    constexpr double eps = 1e-9;

    bool contains = std::ranges::any_of(points, [&](const Vec2& existing) {
        return std::abs(existing.x - p.x) <= eps &&
               std::abs(existing.y - p.y) <= eps;
    });

    if (!contains) {
        points.push_back(p);
        return true;
    }

    return false;
}

bool read_pair(std::istream& in, DXF::GROUP_CODE& code, std::string& value) {

    std::string line;
    std::istringstream iss;

    if(!std::getline(in, line)) {
        return false;
    }
    iss.str(line);
    iss >> code;

    if(!std::getline(in, line)) {
        return false;
    }
    iss.clear();
    iss.str(line);
    iss >> value;

    return true;
}

void parse_line(std::istream& in, Mesh& mesh, DXF::GROUP_CODE& code, std::string& value) {

    Vec2 start;
    Vec2 end;
    start.x = start.y = 0.0;
    end.x = end.y = 0.0;
    
    while(read_pair(in, code, value) && code != 0) {
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

    add_unique(mesh.points, start);
    add_unique(mesh.points, end);

}

void parse_entites(std::istream& in, Mesh& mesh, DXF::GROUP_CODE& code, std::string& value) {

    while(code == DXF::ENTITY_TYPE && value != DXF::ENDSEC) {
        if (code == DXF::ENTITY_TYPE && value == DXF::LINE) {
            parse_line(in, mesh, code, value);
            continue;
        }
    }
}

bool load_dxf(const char* path, Mesh& mesh) {
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
    DXF::GROUP_CODE code = 0;


    while(read_pair(in, code, value)) {

        if(code == DXF::NAME && value == DXF::ENTITIES) {
            read_pair(in, code, value);
            parse_entites(in, mesh, code, value);
            break;
        }

        
    }

    return true;
}