#include "parser/dfx_parser.h"

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

bool read_pair(std::istream& in, int& code, std::string& value) {

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

void parse_line(std::istream& in, Mesh& mesh, int& code, std::string& value) {

    Vec2 start;
    Vec2 end;
    start.x = start.y = 0.0;
    end.x = end.y = 0.0;
    
    while(read_pair(in, code, value) && code != 0) {
        if (code == 10) {
            start.x = std::atof(value.c_str());
        } else if (code == 20) {
            start.y = std::atof(value.c_str());
        } else if (code == 11) {
            end.x = std::atof(value.c_str());
        } else if (code == 21) {
            end.y = std::atof(value.c_str());
        }
    }

    add_unique(mesh.points, start);
    add_unique(mesh.points, end);

}

void parse_entites(std::istream& in, Mesh& mesh, int& code, std::string& value) {

    while(code == 0 && value != "ENDSEC") {
        if (code == 0 && value == "LINE") {
            parse_line(in, mesh, code, value);
            continue;
        }
    }
}

bool load_dfx(const char* path, Mesh& mesh) {
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
    int code = 0;


    while(read_pair(in, code, value)) {

        if(code == 2 && value == "ENTITIES") {
            read_pair(in, code, value);
            parse_entites(in, mesh, code, value);
            break;
        }

        
    }

    return true;
}