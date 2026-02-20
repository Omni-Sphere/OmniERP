#pragma once

#include "Base/BaseUpdateDTO.hpp"
#include <string>

namespace omnisphere::omnierp::dtos {
struct UpdateItemGroup : public omnisphere::omnicore::dtos::BaseUpdateDTO {
  using omnisphere::omnicore::dtos::BaseUpdateDTO::BaseUpdateDTO;
};
} // namespace omnisphere::omnierp::dtos