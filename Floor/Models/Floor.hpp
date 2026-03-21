#pragma once
#include "../../Area/Models/Area.hpp"
#include "Base/BaseModel.hpp"
#include <optional>
#include <vector>

namespace omnisphere::models {
class Floor : public omnisphere::models::BaseModel {
public:
  using omnisphere::models::BaseModel::BaseModel;
  std::vector<Area> Areas;
};
} // namespace omnisphere::models