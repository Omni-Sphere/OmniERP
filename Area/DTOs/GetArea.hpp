#pragma once
#include <optional>
#include <string>

namespace omnisphere::dtos 
{
    struct GetArea
    {
        std::optional<int> Entry;
        std::optional<std::string> Code;
        std::optional<std::string> Name;
        std::optional<std::string> Color;
        std::optional<int> FloorEntry;
    };
}
