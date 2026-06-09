#include <Database.hpp>
#include <DataTable.hpp>
#pragma once
#include <string>
#include <optional>

namespace omnisphere::dtos
{
    struct CreateRole
    {
        std::string Code;
        std::string Name;
        std::optional<int> DepartmentEntry;
        int CreatedBy;
    };

    struct UpdateRole
    {
        int Entry;
        std::optional<std::string> Code;
        std::optional<std::string> Name;
        std::optional<int> DepartmentEntry;
        int UpdatedBy;
    };

    struct GetRole
    {
        std::optional<int> Entry;
        std::optional<std::string> Code;
        std::optional<std::string> Name;
        std::optional<int> DepartmentEntry;
    };
} // namespace omnisphere::dtos
