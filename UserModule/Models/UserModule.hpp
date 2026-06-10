#include <Database.hpp>
#include <DataTable.hpp>
#pragma once
#include <string>
#include <optional>

namespace omnisphere::models
{
    struct UserModule
    {
        int Entry;
        int UserEntry;
        int ModuleEntry;
        bool IsActive;
        int CreatedBy;
        std::string CreateDate;
        std::optional<int> LastUpdatedBy;
        std::optional<std::string> UpdateDate;
    };
} // namespace omnisphere::models
