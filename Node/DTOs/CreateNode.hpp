#pragma once
#include "Node/Enums/NodeType.hpp"
#include "Node/Enums/OperationMode.hpp"
#include "OmniCore/Base/BaseCreateDTO.hpp"
#include <optional>
#include <string>

namespace omnisphere::dtos {
struct CreateNode : public omnisphere::dtos::BaseCreateDTO {
  using omnisphere::dtos::BaseCreateDTO::BaseCreateDTO;
  omnisphere::enums::NodeType NodeType;
  omnisphere::enums::OperationMode OperationMode;
  std::optional<int> CashLimit;
  std::optional<std::string> IPAddress;
  std::optional<bool> ExtendedLog;
};
} // namespace omnisphere::dtos
