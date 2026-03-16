#pragma once

#include "Base/BaseUpdateDTO.hpp"
#include <string>

namespace omnisphere::dtos {
struct UpdateItemGroup : public omnisphere::dtos::BaseUpdateDTO {
  using omnisphere::dtos::BaseUpdateDTO::BaseUpdateDTO;
};
} // namespace omnisphere::dtos