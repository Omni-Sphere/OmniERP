#pragma once

#include "Base/BaseCreateDTO.hpp"
#include <string>

namespace omnicore::dto {
struct CreateItemGroup : public BaseCreateDTO {
  using BaseCreateDTO::BaseCreateDTO;
};
}; // namespace omnicore::dto