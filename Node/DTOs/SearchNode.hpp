#pragma once

#include <optional>
#include <string>

namespace omnisphere::dtos {
struct SearchNode {
  int Entry = 0;
  std::string Code;
  std::string Name;
};
} // namespace omnisphere::dtos
