#pragma once

#include <OmniCore/User/Models/User.hpp>
#include <boost/describe.hpp>
#include <memory>
#include <optional>
#include <string>

namespace omnisphere::models {
class ItemGroup {
public:
  int Entry;
  std::string Code;
  std::string Name;
  int CreatedBy;
  std::string CreateDate;
  std::optional<int> LastUpdatedBy;
  std::optional<std::string> UpdateDate;

  bool IsActive = true;
  std::shared_ptr<omnisphere::models::User> CreatedByUser;
  std::shared_ptr<omnisphere::models::User> LastUpdatedByUser;
};
BOOST_DESCRIBE_STRUCT(ItemGroup, (),
                      (Entry, Code, Name, IsActive, CreatedBy, CreateDate,
                       LastUpdatedBy, UpdateDate))
} // namespace omnisphere::models