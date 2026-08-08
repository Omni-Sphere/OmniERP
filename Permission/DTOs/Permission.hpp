#pragma once
#include <optional>
#include <string>

namespace omnisphere::dtos {
struct GetPermission {
  std::optional<int> Entry;
  std::optional<std::string> Code;
  std::optional<std::string> Name;
  std::optional<int> ModuleEntry;
};
} // namespace omnisphere::dtos
