#include <Database.hpp>
#include <DataTable.hpp>
#pragma once

#include <string>
#include <optional>

namespace omnisphere::dtos
{
    struct SearchNode
    {
        int Entry = 0;
        std::string Code;
        std::string Name;
    };
}
