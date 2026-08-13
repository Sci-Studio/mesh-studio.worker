#include "parser/DxfParser.hpp"
#include "geometry/Point.hpp"
#include "geometry/Arc.hpp"
#include "geometry/Spline.hpp"
#include "parser/DxfCodes.hpp"

#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

using namespace parser;

int dxf::DxfParser::addUnique(std::vector<Point>& points, const Point& point) {
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

void dxf::DxfParser::addConstraintEdge(Mesh& mesh, int i0, int i1) {
    if (i0 != i1) {
        mesh.constraints.push_back(Edge{i0, i1});
    }
}

void dxf::DxfParser::addPolylineConstraints(Mesh& mesh, const std::vector<Point>& poly) {
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

bool dxf::DxfParser::readPair(std::istream& inputFile, GROUP_CODE& code, std::string& value) {

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

void dxf::DxfParser::parseLine(std::istream& inputFile, Mesh& mesh, GROUP_CODE& code, std::string& value) {

    Point start;
    Point end;
    start.x = start.y = 0.0;
    end.x = end.y = 0.0;

    while (readPair(inputFile, code, value) && code != dxf::common::ENTITY_TYPE) {
        if (code == dxf::line::START_X) {
            start.x = std::atof(value.c_str());
        } else if (code == dxf::line::START_Y) {
            start.y = std::atof(value.c_str());
        } else if (code == dxf::line::END_X) {
            end.x = std::atof(value.c_str());
        } else if (code == dxf::line::END_Y) {
            end.y = std::atof(value.c_str());
        }
    }

    addConstraintEdge(mesh, addUnique(mesh.points, start), addUnique(mesh.points, end));
}

void dxf::DxfParser::parseArc(std::istream& inputFile, Mesh& mesh, GROUP_CODE& code, std::string& value) {
    Arc arc;
    ArcDiscretizationOption options(0.01, 10);

    while (readPair(inputFile, code, value) && code != dxf::common::ENTITY_TYPE) {
        if (code == dxf::arc::CENTER_X) {
            arc.centerX = std::stod(value);
        } else if (code == dxf::arc::CENTER_Y) {
            arc.centerY = std::stod(value);
        } else if (code == dxf::arc::RADIUS) {
            arc.radius = std::stod(value);
        } else if (code == dxf::arc::START_ANGLE) {
            arc.startAngleDegree = std::stod(value);
        } else if (code == dxf::arc::END_ANGLE) {
            arc.endAngleDegree = std::stod(value);
        }
    }

    if (arc.radius <= 0.0) {
        return;
    }

    std::vector<Point> polyline = arc.discretizeArc(options);

    addPolylineConstraints(mesh, polyline);
}

void dxf::DxfParser::parseSpline(std::istream& inputFile, Mesh& mesh, GROUP_CODE& code, std::string& value) {

    Spline spline;
    Point current;
    GeometryTolerance geometryTolerance;
    bool haveX = false;

    while (readPair(inputFile, code, value) && code != dxf::common::ENTITY_TYPE) {
        if (code == dxf::spline::DEGREE) {
            spline.degree = std::stoi(value);
        } else if (code == dxf::spline::CONTROL_X) {
            current.x = std::stod(value);
            haveX = true;
        } else if (code == dxf::spline::CONTROL_Y && haveX) {
            current.y = std::stod(value);
            spline.controlPoints.push_back(current);
            haveX = false;
        } else if (code == dxf::spline::KNOT) {
            spline.knots.push_back(std::stod(value));
        }
    }

    const auto polyline = spline.discretizeSpline(geometryTolerance);

    addPolylineConstraints(mesh, polyline);
}

void dxf::DxfParser::parseEntites(std::istream& inputFile, Mesh& mesh, GROUP_CODE& code, std::string& value) {

    while (code == dxf::common::ENTITY_TYPE && value != dxf::common::ENDSEC) {
        if (value == dxf::common::LINE) {
            parseLine(inputFile, mesh, code, value);
            continue;
        }
        if (value == dxf::common::ARC) {
            parseArc(inputFile, mesh, code, value);
            continue;
        }
        if (value == dxf::common::SPLINE) {
            parseSpline(inputFile, mesh, code, value);
            continue;
        }
        // Skip unsupported entities until the next group-0 marker.
        while (readPair(inputFile, code, value) && code != dxf::common::ENTITY_TYPE) {
        }
    }
}

bool dxf::DxfParser::loadMesh(const char* path, Mesh& mesh) {
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
        if (code == dxf::common::NAME && value == dxf::common::ENTITIES) {
            readPair(inputFile, code, value);
            parseEntites(inputFile, mesh, code, value);
            break;
        }
    }

    return true;
}
