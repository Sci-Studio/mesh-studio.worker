#include "file-writer/SvgFileWriter.hpp"
#include "geometry/Mesh.hpp"
#include "geometry/GeometryUtils.hpp"

#include <fstream>

namespace writer {

    bool SvgFileWriter::saveMeshToOutputFile(const geometry::Mesh& mesh) {
        if (mesh.points.empty()) {
            return false;
          }
        
        geometry::BoundingBox box = getboundingBox(mesh.points);
    
        const double pad = 0.05 * std::max(box.maxX - box.minX, box.maxY - box.minY);
        const double usePad = (pad > 0.0) ? pad : 0.1;
    
        double padMaxX = box.maxX + usePad; 
        double padMaxY = box.maxY + usePad; 
        double padMinX = box.minX - usePad; 
        double padMinY = box.minY - usePad; 
    
        const double width = padMaxX - padMinX;
        const double height = padMaxY - padMinY;

        std::ofstream out(this->outputPath());
        if (!out) {
          return false;
        }
    
        out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
        out << "<svg xmlns=\"http://www.w3.org/2000/svg\" "
            << "viewBox=\"" << padMinX << " " << padMinY << " " << width << " " << height << "\" "
            << "width=\"800\" height=\"800\">\n";
        
        out << "  <g stroke=\"#bbb\" stroke-width=\"0.1\" fill=\"none\">\n";
        for (auto triangle : mesh.triangles) {
          const geometry::Point& a = mesh.points[triangle.vertices[0]];
          const geometry::Point& b = mesh.points[triangle.vertices[1]];
          const geometry::Point& c = mesh.points[triangle.vertices[2]];
          out << "    <polygon points=\"" << a.x << "," << a.y << " "
                                          << b.x << "," << b.y << " "
                                          << c.x << "," << c.y << "\" />\n";
        }
        out << "  </g>\n";
    
        out << "  <g stroke=\"#db3c1d\" stroke-width=\"0.2\" fill=\"none\">\n";
        for (auto edge : mesh.constraints) {
          if (edge.a < 0 || edge.b < 0 ||
            edge.a >= static_cast<int>(mesh.points.size()) ||
            edge.b >= static_cast<int>(mesh.points.size())) {
          continue;
        }
          const geometry::Point& a = mesh.points[edge.a];
          const geometry::Point& b = mesh.points[edge.b];
          out << "    <polygon points=\"" << a.x << "," << a.y << " "
                                          << b.x << "," << b.y << "\" />\n";
        }
        out << "  </g>\n";
    
        out << "  <g stroke=\"none\" fill=\"#1ddbdb\">\n";
        // const double r = 0.015 * std::max(width, height);
        const double r = 0.2;
    
        for (auto point : mesh.points) {
          out << "    <circle cx=\"" << point.x << "\" cy=\"" << point.y << "\" r=\"" << r
              << "\" />\n";
        }
        out << "  </g>\n";
        out << "</svg>\n";
    
        return static_cast<bool>(out);
    }
}