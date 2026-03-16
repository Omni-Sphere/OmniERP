#pragma once

#include "Base/BaseCreateDTO.hpp"
#include <string>

namespace omnisphere::dtos {
struct CreateItemGroup : public omnisphere::dtos::BaseCreateDTO {
  using omnisphere::dtos::BaseCreateDTO::BaseCreateDTO;
};
}; // namespace omnisphere::dtos