#pragma once
#include "Base/BaseUpdateDTO.hpp"
#include <string>

namespace omnisphere::dtos 
{
    struct UpdateArea : public omnisphere::dtos::BaseUpdateDTO {
        using omnisphere::dtos::BaseUpdateDTO::BaseUpdateDTO;
        std::optional<int> Entry;
        std::optional<std::string> Color;        
        std::optional<int> Capacity;
        std::optional<int> FloorEntry;
        std::optional<std::string> Icon;
    };
};
