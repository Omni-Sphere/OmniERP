#pragma once

#include "Base/BaseUpdateDTO.hpp"
#include <string>

namespace omnisphere::dtos {
struct UpdateItemBrand : public omnisphere::dtos::BaseUpdateDTO {
  using omnisphere::dtos::BaseUpdateDTO::BaseUpdateDTO;
};
} // namespace omnisphere::dtos