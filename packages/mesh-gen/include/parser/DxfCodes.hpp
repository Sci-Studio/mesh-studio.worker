#pragma once

namespace parser {
    namespace dxf {

        typedef unsigned int GROUP_CODE;
        typedef const char* ENTITY_NAME;

        // Group codes
        constexpr GROUP_CODE ENTITY_TYPE = 0;
        constexpr GROUP_CODE NAME = 2;
        constexpr GROUP_CODE START_X = 10;   // LINE start / ARC center / SPLINE control X
        constexpr GROUP_CODE START_Y = 20;
        constexpr GROUP_CODE START_Z = 30;
        constexpr GROUP_CODE END_X = 11;
        constexpr GROUP_CODE END_Y = 21;
        constexpr GROUP_CODE END_Z = 31;
        constexpr GROUP_CODE RADIUS = 40;
        constexpr GROUP_CODE START_ANGLE = 50;  // degrees, CCW from +X
        constexpr GROUP_CODE END_ANGLE = 51;

        // Entity Names
        inline constexpr ENTITY_NAME ENTITIES = "ENTITIES";
        inline constexpr ENTITY_NAME ENDSEC = "ENDSEC";
        inline constexpr ENTITY_NAME LINE = "LINE";
        inline constexpr ENTITY_NAME ARC = "ARC";
        inline constexpr ENTITY_NAME SPLINE = "SPLINE";

    }
}
