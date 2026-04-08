#pragma once

#include <optional>
#include <string>

namespace omnisphere::dtos {
    struct GetStoreConfig {
        std::optional<int> Entry;
        std::optional<std::string> Code;
        std::optional<std::string> Name;
    };
}
