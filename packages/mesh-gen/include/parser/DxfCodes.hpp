#pragma once

namespace parser {
    namespace dxf {

        typedef unsigned int GROUP_CODE;
        typedef const char* ENTITY_NAME; 
    
        // Group codes
        constexpr GROUP_CODE ENTITY_TYPE = 0;
        constexpr GROUP_CODE NAME = 2;
        constexpr GROUP_CODE START_X =    10;
        constexpr GROUP_CODE START_Y =    20;
        constexpr GROUP_CODE START_Z =    30;
        constexpr GROUP_CODE END_X =      11;
        constexpr GROUP_CODE END_Y =      21;
        constexpr GROUP_CODE END_Z =      31;
    
    
        // Entity Names
        inline constexpr ENTITY_NAME ENTITIES = "ENTITIES";
        inline constexpr ENTITY_NAME ENDSEC = "ENDSEC";
        inline constexpr ENTITY_NAME LINE = "LINE";
        inline constexpr ENTITY_NAME SPLINE = "SPLINE";
    
    }
}
