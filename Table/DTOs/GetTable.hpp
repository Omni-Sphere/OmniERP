#pragma once
#include <optional>
#include <string>

namespace omnisphere::dtos {
    struct GetTable {
        std::optional<int> Entry;
        std::optional<std::string> Code;
        std::optional<std::string> Name;
        std::optional<int> AreaEntry;
        std::optional<int> FloorEntry;
    };
}
