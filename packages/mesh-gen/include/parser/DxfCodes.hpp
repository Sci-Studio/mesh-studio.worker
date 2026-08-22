#pragma once

namespace parser {
    namespace dxf {

        using GROUP_CODE = unsigned int;
        using ENTITY_NAME = const char*;

        namespace common {
            constexpr GROUP_CODE ENTITY_TYPE = 0;
            constexpr GROUP_CODE NAME = 2;

            inline constexpr ENTITY_NAME ENTITIES = "ENTITIES";
            inline constexpr ENTITY_NAME ENDSEC = "ENDSEC";
            inline constexpr ENTITY_NAME LINE = "LINE";
            inline constexpr ENTITY_NAME ARC = "ARC";
            inline constexpr ENTITY_NAME CIRCLE = "CIRCLE";
            inline constexpr ENTITY_NAME SPLINE = "SPLINE";
        } // namespace common

        namespace line {
            constexpr GROUP_CODE START_X = 10;
            constexpr GROUP_CODE START_Y = 20;
            constexpr GROUP_CODE END_X = 11;
            constexpr GROUP_CODE END_Y = 21;
        } // namespace line

        namespace arc {
            constexpr GROUP_CODE CENTER_X = 10;
            constexpr GROUP_CODE CENTER_Y = 20;
            constexpr GROUP_CODE RADIUS = 40;
            constexpr GROUP_CODE START_ANGLE = 50; // degrees, CCW from +X
            constexpr GROUP_CODE END_ANGLE = 51;
        } // namespace arc

        namespace circle {
            constexpr GROUP_CODE CENTER_X = 10;
            constexpr GROUP_CODE CENTER_Y = 20;
            constexpr GROUP_CODE RADIUS = 40;
        } // namespace circle

        namespace spline {
            constexpr GROUP_CODE CONTROL_X = 10;
            constexpr GROUP_CODE CONTROL_Y = 20;
            constexpr GROUP_CODE KNOT = 40;
            constexpr GROUP_CODE WEIGHT = 41;
            constexpr GROUP_CODE DEGREE = 71;
        } // namespace spline

    } // namespace dxf
} // namespace parser
