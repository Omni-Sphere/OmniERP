#pragma once
#include <optional>
#include <string>

namespace omnisphere::dtos {
struct CreateDepartment {
  std::string Code;
  std::string Name;
  int CreatedBy;
};

struct UpdateDepartment {
  int Entry;
  std::optional<std::string> Code;
  std::optional<std::string> Name;
  std::optional<bool> IsActive;
  int UpdatedBy;
};

struct GetDepartment {
  std::optional<int> Entry;
  std::optional<std::string> Code;
  std::optional<std::string> Name;
};
} // namespace omnisphere::dtos
