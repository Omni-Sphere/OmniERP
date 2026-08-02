#include <Database.hpp>
#include <DataTable.hpp>
#pragma once

#include <string>
#include <optional>

namespace omnisphere::dtos
{
    struct GetStore
    {       
        std::optional<int> Entry;
        std::optional<std::string> Code;
        std::optional<std::string> Name;        
    };
}
