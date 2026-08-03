#pragma once

#include "DataStructures.hpp"

#include <string>


// Returns true if every triangle is strictly CCW.
bool all_triangles_ccw(const Mesh& mesh);

// Brute-force: no mesh point lies inside any triangle's circumcircle.
bool is_delaunay(const Mesh& mesh);

// Write SVG of triangle edges. Returns false on I/O failure.
bool write_svg(const Mesh& mesh, const std::string& path);



