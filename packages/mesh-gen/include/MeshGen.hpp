#pragma once

#include "geometry/Mesh.hpp"

#include <string>

int writeMeshToFile(const geometry::Mesh& mesh, const std::string& path);
int generateMesh(const char* inputPath);
