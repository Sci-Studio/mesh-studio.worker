#include "parser/DxfParser.hpp"
#include "geometry/Point.hpp"

#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

using namespace parser::dxf;

namespace {

constexpr double kPi = 3.14159265358979323846;
constexpr double kDegToRad = kPi / 180.0;
// Samples per 90° of arc (quarter-circle corners get this many segments)
constexpr int kArcSamplesPerQuarter = 8;

double degToRad(double degrees) {
    return degrees * kDegToRad;
}

// Positive CCW span from startDeg to endDeg in [0, 360]
double ccwSpanDegrees(double startDeg, double endDeg) {
    double span = endDeg - startDeg;
    while (span <= 0.0) {
        span += 360.0;
    }
    while (span > 360.0) {
        span -= 360.0;
    }
    return span;
}

Point pointOnCircle(double cx, double cy, double radius, double angleDeg) {
    const double a = degToRad(angleDeg);
    Point p;
    p.x = cx + radius * std::cos(a);
    p.y = cy + radius * std::sin(a);
    p.type = PointType::NORMAL;
    return p;
}

}  // namespace

int DxfParser::addUnique(std::vector<Point>& points, const Point& point) {
    constexpr double eps = 1e-9;

    for (size_t i = 0; i < points.size(); ++i) {
        const Point& existing = points[i];
        if (std::abs(existing.x - point.x) <= eps &&
            std::abs(existing.y - point.y) <= eps) {
            return static_cast<int>(i);
        }
    }

    Point p = point;
    p.index = static_cast<int>(points.size());
    p.type = PointType::NORMAL;
    points.push_back(p);
    return p.index;
}

void DxfParser::addConstraintEdge(Mesh& mesh, int i0, int i1) {
    if (i0 != i1) {
        mesh.constraints.push_back(Edge{i0, i1});
    }
}

void DxfParser::addPolylineConstraints(Mesh& mesh, const std::vector<Point>& poly) {
    if (poly.size() < 2) {
        return;
    }
    int prev = addUnique(mesh.points, poly[0]);
    for (size_t i = 1; i < poly.size(); ++i) {
        const int curr = addUnique(mesh.points, poly[i]);
        addConstraintEdge(mesh, prev, curr);
        prev = curr;
    }
}

bool DxfParser::readPair(std::istream& inputFile, GROUP_CODE& code, std::string& value) {

    std::string line;
    std::istringstream istringstream;

    if (!std::getline(inputFile, line)) {
        return false;
    }
    istringstream.str(line);
    istringstream >> code;

    if (!std::getline(inputFile, line)) {
        return false;
    }
    istringstream.clear();
    istringstream.str(line);
    istringstream >> value;

    return true;
}

void DxfParser::parseLine(std::istream& inputFile, Mesh& mesh, GROUP_CODE& code, std::string& value) {

    Point start;
    Point end;
    start.x = start.y = 0.0;
    end.x = end.y = 0.0;

    while (readPair(inputFile, code, value) && code != dxf::ENTITY_TYPE) {
        if (code == dxf::START_X) {
            start.x = std::atof(value.c_str());
        } else if (code == dxf::START_Y) {
            start.y = std::atof(value.c_str());
        } else if (code == dxf::END_X) {
            end.x = std::atof(value.c_str());
        } else if (code == dxf::END_Y) {
            end.y = std::atof(value.c_str());
        }
    }

    addConstraintEdge(mesh, addUnique(mesh.points, start), addUnique(mesh.points, end));
}

void DxfParser::parseArc(std::istream& inputFile, Mesh& mesh, GROUP_CODE& code, std::string& value) {
    double cx = 0.0;
    double cy = 0.0;
    double radius = 0.0;
    double startAngle = 0.0;
    double endAngle = 0.0;

    while (readPair(inputFile, code, value) && code != dxf::ENTITY_TYPE) {
        if (code == dxf::START_X) {
            cx = std::atof(value.c_str());
        } else if (code == dxf::START_Y) {
            cy = std::atof(value.c_str());
        } else if (code == dxf::RADIUS) {
            radius = std::atof(value.c_str());
        } else if (code == dxf::START_ANGLE) {
            startAngle = std::atof(value.c_str());
        } else if (code == dxf::END_ANGLE) {
            endAngle = std::atof(value.c_str());
        }
    }

    if (radius <= 0.0) {
        return;
    }

    const double span = ccwSpanDegrees(startAngle, endAngle);
    int segments = static_cast<int>(std::lround(span / 90.0 * kArcSamplesPerQuarter));
    if (segments < 1) {
        segments = 1;
    }

    std::vector<Point> poly;
    poly.reserve(static_cast<size_t>(segments) + 1);
    for (int i = 0; i <= segments; ++i) {
        const double t = static_cast<double>(i) / static_cast<double>(segments);
        const double angle = startAngle + t * span;
        poly.push_back(pointOnCircle(cx, cy, radius, angle));
    }

    addPolylineConstraints(mesh, poly);
}

void DxfParser::parseSpline(std::istream& inputFile, Mesh& mesh, GROUP_CODE& code, std::string& value) {
    // Control polyline: successive group 10/20 pairs (degree-1 FreeCAD exports are collinear).
    std::vector<Point> control;
    Point current;
    bool haveX = false;

    while (readPair(inputFile, code, value) && code != dxf::ENTITY_TYPE) {
        if (code == dxf::START_X) {
            current.x = std::atof(value.c_str());
            haveX = true;
        } else if (code == dxf::START_Y && haveX) {
            current.y = std::atof(value.c_str());
            current.type = PointType::NORMAL;
            control.push_back(current);
            haveX = false;
        }
    }

    if (control.size() < 2) {
        return;
    }

    // Collapse nearly collinear control polygons to endpoints (common for degree-1 exports).
    constexpr double collinearEps = 1e-9;
    bool collinear = true;
    const Point& a = control.front();
    const Point& b = control.back();
    for (size_t i = 1; i + 1 < control.size(); ++i) {
        const Point& p = control[i];
        const double cross = (b.x - a.x) * (p.y - a.y) - (b.y - a.y) * (p.x - a.x);
        if (std::abs(cross) > collinearEps) {
            collinear = false;
            break;
        }
    }

    if (collinear) {
        addConstraintEdge(mesh, addUnique(mesh.points, a), addUnique(mesh.points, b));
    } else {
        addPolylineConstraints(mesh, control);
    }
}

void DxfParser::parseEntites(std::istream& inputFile, Mesh& mesh, GROUP_CODE& code, std::string& value) {

    while (code == dxf::ENTITY_TYPE && value != dxf::ENDSEC) {
        if (value == dxf::LINE) {
            parseLine(inputFile, mesh, code, value);
            continue;
        }
        if (value == dxf::ARC) {
            parseArc(inputFile, mesh, code, value);
            continue;
        }
        if (value == dxf::SPLINE) {
            parseSpline(inputFile, mesh, code, value);
            continue;
        }
        // Skip unsupported entities until the next group-0 marker.
        while (readPair(inputFile, code, value) && code != dxf::ENTITY_TYPE) {
        }
    }
}

bool DxfParser::loadMesh(const char* path, Mesh& mesh) {
    mesh.points.clear();
    mesh.triangles.clear();
    mesh.constraints.clear();

    std::ifstream inputFile(path);
    if (!inputFile) {
      std::cerr << "Failed to open .dxf file: " << path << std::endl;
      return false;
    }

    std::string value;
    GROUP_CODE code = 0;

    while (readPair(inputFile, code, value)) {
        if (code == dxf::NAME && value == dxf::ENTITIES) {
            readPair(inputFile, code, value);
            parseEntites(inputFile, mesh, code, value);
            break;
        }
    }

    return true;
}
