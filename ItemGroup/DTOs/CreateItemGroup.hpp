#pragma once

#include "Base/BaseCreateDTO.hpp"
#include <string>

namespace omnisphere::omnierp::dtos {
struct CreateItemGroup : public omnisphere::omnicore::dtos::BaseCreateDTO {
  using omnisphere::omnicore::dtos::BaseCreateDTO::BaseCreateDTO;
};
}; // namespace omnisphere::omnierp::dtos