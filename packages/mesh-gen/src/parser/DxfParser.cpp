#include "parser/DxfParser.hpp"
#include "geometry/Mesh.hpp"
#include "geometry/Point.hpp"
#include "geometry/Arc.hpp"
#include "geometry/Spline.hpp"
#include "parser/DxfCodes.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using namespace parser;
using namespace geometry;

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

    mesh.addConstraintEdge(mesh.addUnique( start ), mesh.addUnique(end));
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

    auto polyline = arc.discretizeArc(options);

    mesh.addPolylineConstraints(polyline);
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

    mesh.addPolylineConstraints(polyline);
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
    mesh.clear();

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
