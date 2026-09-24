#pragma once
#include "Area/Models/Area.hpp"
#include <OmniCore/Base/BaseModel.hpp>
#include <boost/describe.hpp>
#include <optional>
#include <vector>

namespace omnisphere::models {
class Floor : public omnisphere::models::BaseModel {
public:
  Floor() = default;
  using omnisphere::models::BaseModel::BaseModel;
  std::vector<Area> Areas;
};
BOOST_DESCRIBE_STRUCT(Floor, (omnisphere::models::BaseModel), (Areas))
} // namespace omnisphere::models