#pragma once

#include "DTOs/CreateItemGroup.hpp"
#include "DTOs/GetItemGroup.hpp"
#include "DTOs/UpdateItemGroup.hpp"
#include "Database.hpp"

#include <stdexcept>
#include <string>
#include <vector>

namespace omnisphere::omnierp::repositories {
class ItemGroup {
private:
  std::shared_ptr<omnisphere::omnidata::services::Database> database;
  int GetCurrentSequence() const;
  bool UpdateItemGroupsSequence() const;

public:
  explicit ItemGroup(
      std::shared_ptr<omnisphere::omnidata::services::Database> Database);
  ~ItemGroup() {};

  bool Create(
      const omnisphere::omnierp::dtos::CreateItemGroup &createItemGroup) const;
  bool Update(
      const omnisphere::omnierp::dtos::UpdateItemGroup &updateItemGroup) const;
  omnisphere::omnidata::types::DataTable ReadAll() const;
  omnisphere::omnidata::types::DataTable
  Read(const omnisphere::omnierp::dtos::GetItemGroup) const;
};
} // namespace omnisphere::omnierp::repositories