#include <Database.hpp>
#include <DataTable.hpp>
#pragma once
#include <string>
#include <optional>

namespace omnisphere::dtos
{
    struct GetPermission
    {
        std::optional<int> Entry;
        std::optional<std::string> Code;
        std::optional<std::string> Name;
        std::optional<int> ModuleEntry;
    };
} // namespace omnisphere::dtos
