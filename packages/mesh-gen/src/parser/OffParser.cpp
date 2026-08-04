#include "parser/OffParser.hpp"

#include <fstream>
#include <iostream>
#include <string>

using namespace parser::off;

bool OffParser::loadMesh(const char* path, Mesh& mesh) {
    mesh.points.clear();
    mesh.triangles.clear();

    std::ifstream in(path);

    if (!in) {
        std::cerr << "Failed to open .off file: " << path << std::endl;
        return false;
    }

    std::string token;

    if(!(in >> token) || token != "OFF") {
        std::cerr << "Invalid .off file: " << path << std::endl;
        return false;
    }

    int n_verts = 0;
    int n_faces = 0;
    int n_edges = 0;

    if(!(in >> n_verts >> n_faces >> n_edges) || n_verts < 0) {
        return false;
    }

    mesh.points.reserve(static_cast<size_t>(n_verts));
    
    for(int i = 0; i < n_verts; i++) {
        double x = 0.0;
        double y = 0.0;
        double z = 0.0;
        in >> x >> y >> z;
        
        Vec2 p;
        p.x = x;
        p.y = y;
        
        mesh.points.push_back(p);
    }

    in.close();
    return true;
}