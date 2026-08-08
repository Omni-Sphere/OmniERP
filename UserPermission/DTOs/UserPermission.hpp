#pragma once
#include <optional>
#include <string>

namespace omnisphere::dtos {
struct CreateUserPermission {
  int UserEntry;
  int PermissionEntry;
  bool IsActive;
  int CreatedBy;
};

struct UpdateUserPermission {
  int Entry;
  std::optional<bool> IsActive;
  int UpdatedBy;
};

struct GetUserPermission {
  std::optional<int> Entry;
  std::optional<int> UserEntry;
  std::optional<int> PermissionEntry;
};
} // namespace omnisphere::dtos
