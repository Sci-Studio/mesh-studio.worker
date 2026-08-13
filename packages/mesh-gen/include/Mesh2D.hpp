#pragma once

#include "geometry/Mesh.hpp"

#include <string>

bool allTrianglesCcw(const geometry::Mesh& mesh);
bool isDelaunay(const geometry::Mesh& mesh);
bool writeSvg(const geometry::Mesh& mesh, const std::string& path);
