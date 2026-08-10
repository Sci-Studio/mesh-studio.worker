#pragma once

#include "DataStructures.hpp"

#include <string>

int writeMeshToFile(const Mesh& mesh, const std::string& path);

int generateMesh(const char* inputPath);