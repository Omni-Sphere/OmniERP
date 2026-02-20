#pragma once

#include "Base/BaseCreateDTO.hpp"
#include <string>

namespace omnisphere::omnierp::dtos {
struct CreateItemBrand : public omnisphere::omnicore::dtos::BaseCreateDTO {
  using omnisphere::omnicore::dtos::BaseCreateDTO::BaseCreateDTO;
};
} // namespace omnisphere::omnierp::dtos