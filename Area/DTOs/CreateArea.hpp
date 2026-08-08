#pragma once
#include "OmniCore/Base/BaseCreateDTO.hpp"

namespace omnisphere::dtos {
struct CreateArea : public omnisphere::dtos::BaseCreateDTO {
  using omnisphere::dtos::BaseCreateDTO::BaseCreateDTO;
  std::string Color;
  int Capacity;
  int FloorEntry;
  std::string Icon;
};
} // namespace omnisphere::dtos
