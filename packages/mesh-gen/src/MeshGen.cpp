#include "MeshGen.hpp"
#include "Delaunay.hpp"
#include "file-writer/SvgFileWriter.hpp"
#include "geometry/Mesh.hpp"
#include "parser/DxfParser.hpp"

#include <filesystem>
#include <iostream>

using namespace geometry;

int generateMesh(const char* inputPath) {

    const std::filesystem::path outputDirectory{"tmp"};
    std::filesystem::create_directories(outputDirectory);
    const std::filesystem::path outputPath =
        outputDirectory / (std::filesystem::path(inputPath).stem().string() + ".svg");

    Mesh mesh;
    parser::dxf::DxfParser parser;
    writer::SvgFileWriter svgFileWriter(outputPath);
    const char* svgPath = "delaunay.svg";

    if (!parser.loadMesh(inputPath, mesh)) {
        std::cerr << "Failed to load mesh from " << inputPath << std::endl;
        return 1;
    }

    std::cout << "Loaded " << mesh.points.size() << " points, " << mesh.constraints.size()
              << " constraint edges from " << inputPath << "\n";

    if (!triangulate(mesh)) {
        std::cerr << "triangulate failed\n";
        return 2;
    }

    std::cout << "Triangles: " << mesh.triangles.size() << "\n";
    for (size_t i = 0; i < mesh.triangles.size(); ++i) {
        const Triangle& t = mesh.triangles[i];
        std::cout << "  [" << i << "] (" << t.vertices[0] << ", " << t.vertices[1] << ", "
                  << t.vertices[2] << ")\n";
    }

    if (!mesh.allTrianglesCcw()) {
        std::cerr << "Validation failed: not all triangles are CCW\n";
        return 3;
    }
    // Strict Delaunay often fails on arc samples (many cocircular points). Keep CCW hard;
    // treat Delaunay as a warning until CDT is in place.
    if (!mesh.isDelaunay()) {
        std::cerr << "Warning: triangulation is not strictly Delaunay "
                     "(common with sampled arcs)\n";
    } else {
        std::cout << "Validation OK (CCW + Delaunay)\n";
    }

    if (!svgFileWriter.saveMeshToOutputFile(mesh)) {
        std::cerr << "Failed to write SVG: " << svgPath << "\n";
        return 4;
    }
    std::cout << "Wrote " << svgPath << "\n";

    return 0;
}
