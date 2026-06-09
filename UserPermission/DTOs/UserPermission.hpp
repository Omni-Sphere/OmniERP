#include <Database.hpp>
#include <DataTable.hpp>
#pragma once
#include <string>
#include <optional>

namespace omnisphere::dtos
{
    struct CreateUserPermission
    {
        int UserEntry;
        int PermissionEntry;
        bool IsAllowed;
        int CreatedBy;
    };

    struct UpdateUserPermission
    {
        int Entry;
        std::optional<bool> IsAllowed;
        int UpdatedBy;
    };

    struct GetUserPermission
    {
        std::optional<int> Entry;
        std::optional<int> UserEntry;
        std::optional<int> PermissionEntry;
    };
} // namespace omnisphere::dtos
