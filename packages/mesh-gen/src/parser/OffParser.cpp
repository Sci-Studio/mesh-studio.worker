#include "parser/OffParser.hpp"
#include "geometry/Point.hpp"

#include <fstream>
#include <iostream>
#include <string>

using namespace parser::off;

bool OffParser::loadMesh(const char* path, Mesh& mesh) {
    mesh.points.clear();
    mesh.triangles.clear();

    std::ifstream inputFile(path);

    if (!inputFile) {
        std::cerr << "Failed to open .off file: " << path << std::endl;
        return false;
    }

    std::string token;

    if(!(inputFile >> token) || token != "OFF") {
        std::cerr << "Invalid .off file: " << path << std::endl;
        return false;
    }

    int numberOfVertices = 0;
    int numberOfFaces = 0;
    int numberOfEdges = 0;

    if(!(inputFile >> numberOfVertices >> numberOfFaces >> numberOfEdges) || numberOfVertices < 0) {
        return false;
    }

    mesh.points.reserve(static_cast<size_t>(numberOfVertices));
    
    for(int i = 0; i < numberOfVertices; i++) {
        double x = 0.0;
        double y = 0.0;
        double z = 0.0;
        inputFile >> x >> y >> z;
        
        Point p;
        p.x = x;
        p.y = y;
        
        mesh.points.push_back(p);
    }

    inputFile.close();
    return true;
}