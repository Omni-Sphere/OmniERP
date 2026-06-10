#include <Database.hpp>
#include <DataTable.hpp>
#pragma once
#include <string>
#include <optional>

namespace omnisphere::dtos
{
    struct CreateUserModule
    {
        int UserEntry;
        int ModuleEntry;
        bool IsActive;
        int CreatedBy;
    };

    struct UpdateUserModule
    {
        int Entry;
        std::optional<bool> IsActive;
        int UpdatedBy;
    };

    struct GetUserModule
    {
        std::optional<int> Entry;
        std::optional<int> UserEntry;
        std::optional<int> ModuleEntry;
    };
} // namespace omnisphere::dtos
