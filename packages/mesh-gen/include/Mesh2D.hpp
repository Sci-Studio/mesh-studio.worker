#pragma once

#include "DataStructures.hpp"

#include <string>


// Returns true if every triangle is strictly CCW.
bool allTrianglesCcw(const Mesh& mesh);

// Brute-force: no mesh point lies inside any triangle's circumcircle.
bool isDelaunay(const Mesh& mesh);

// Write SVG of triangle edges. Returns false on I/O failure.
bool writeSvg(const Mesh& mesh, const std::string& path);



