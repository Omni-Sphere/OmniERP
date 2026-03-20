#pragma once
#include <optional>
#include <string>
namespace omnisphere::dtos 
{
    struct GetFloor 
    {
        std::optional<int> Entry;
        std::optional<std::string> Code;
    };
}
