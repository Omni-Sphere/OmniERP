#include <Database.hpp>
#include <DataTable.hpp>
#pragma once
#include <string>
#include <optional>

namespace omnisphere::models
{
    struct UserPermission
    {
        int Entry;
        int UserEntry;
        int PermissionEntry;
        bool IsActive;
        int CreatedBy;
        std::string CreateDate;
        std::optional<int> LastUpdatedBy;
        std::optional<std::string> UpdateDate;
    };
} // namespace omnisphere::models
