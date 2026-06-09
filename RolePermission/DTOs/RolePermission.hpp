#include <Database.hpp>
#include <DataTable.hpp>
#pragma once
#include <string>
#include <optional>

namespace omnisphere::dtos
{
    struct CreateRolePermission
    {
        int RoleEntry;
        int PermissionEntry;
        bool IsAllowed;
        int CreatedBy;
    };

    struct UpdateRolePermission
    {
        int Entry;
        std::optional<bool> IsAllowed;
        int UpdatedBy;
    };

    struct GetRolePermission
    {
        std::optional<int> Entry;
        std::optional<int> RoleEntry;
        std::optional<int> PermissionEntry;
    };
} // namespace omnisphere::dtos
