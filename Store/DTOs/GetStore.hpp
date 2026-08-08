#pragma once

#include <optional>
#include <string>

namespace omnisphere::dtos {
struct GetStore {
  std::optional<int> Entry;
  std::optional<std::string> Code;
  std::optional<std::string> Name;
};
} // namespace omnisphere::dtos
