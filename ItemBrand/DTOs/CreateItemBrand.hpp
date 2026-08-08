#pragma once

#include "OmniCore/Base/BaseCreateDTO.hpp"
#include <string>

namespace omnisphere::dtos {
struct CreateItemBrand : public omnisphere::dtos::BaseCreateDTO {
  using omnisphere::dtos::BaseCreateDTO::BaseCreateDTO;
};
} // namespace omnisphere::dtos