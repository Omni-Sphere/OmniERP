#include <Database.hpp>
#include <DataTable.hpp>
#pragma once
#include <string>
#include <optional>

namespace omnisphere::dtos {
struct GetModule {
    std::optional<int> Entry;
    std::optional<std::string> Code;
    std::optional<std::string> Name;
    std::optional<int> DepartmentEntry;
};
} // namespace omnisphere::dtos
