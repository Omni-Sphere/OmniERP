#pragma once

#include <string>

namespace omnisphere::dtos {
    struct GetBusinessConfig {
        int Entry = 0;
        std::string Code = "";
        std::string Name = "";
    };
}
