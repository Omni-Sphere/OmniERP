#pragma once
#include "Base/BaseUpdateDTO.hpp"
#include <optional>
#include <string>

namespace omnisphere::dtos {
    struct UpdateTable : public omnisphere::dtos::BaseUpdateDTO {
        using omnisphere::dtos::BaseUpdateDTO::BaseUpdateDTO;
        int Entry;
        std::optional<int> Capacity;
        std::optional<std::string> Type;
        std::optional<int> AreaEntry;
        std::optional<int> FloorEntry;
    };
}
