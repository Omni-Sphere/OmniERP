#pragma once

#include <optional>
#include <string>

namespace omnisphere::omnierp::dtos {
struct GetItemBrand {
  std::optional<int> Entry;
  std::optional<std::string> Code;
  std::optional<std::string> Name;
};
} // namespace omnisphere::omnierp::dtos