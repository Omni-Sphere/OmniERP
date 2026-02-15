#pragma once

#include "Base/BaseUpdateDTO.hpp"
#include <string>

namespace omnicore::dto {
struct UpdateItemGroup : public BaseUpdateDTO {
  using BaseUpdateDTO::BaseUpdateDTO;
};
} // namespace omnicore::dto