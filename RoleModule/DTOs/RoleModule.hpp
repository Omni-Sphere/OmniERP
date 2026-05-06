#include <Database.hpp>
#include <DataTable.hpp>
#pragma once
#include <string>
#include <optional>

namespace omnisphere::dtos {
struct CreateRoleModule {
    int RoleEntry;
    int ModuleEntry;
    bool IsAllowed;
    int CreatedBy;
};

struct UpdateRoleModule {
    int Entry;
    std::optional<bool> IsAllowed;
    int UpdatedBy;
};

struct GetRoleModule {
    std::optional<int> Entry;
    std::optional<int> RoleEntry;
    std::optional<int> ModuleEntry;
};
} // namespace omnisphere::dtos
