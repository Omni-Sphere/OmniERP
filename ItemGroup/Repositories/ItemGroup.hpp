#pragma once

#include "DTOs/CreateItemGroup.hpp"
#include "DTOs/GetItemGroup.hpp"
#include "DTOs/UpdateItemGroup.hpp"
#include "Database.hpp"

#include <stdexcept>
#include <string>
#include <vector>

namespace omnisphere::repositories {
class ItemGroup {
private:
  std::shared_ptr<omnisphere::services::Database> database;
  int GetCurrentSequence() const;
  bool UpdateItemGroupsSequence() const;

public:
  explicit ItemGroup(std::shared_ptr<omnisphere::services::Database> Database);
  ~ItemGroup() {};

  bool Create(const omnisphere::dtos::CreateItemGroup &createItemGroup) const;
  bool Update(const omnisphere::dtos::UpdateItemGroup &updateItemGroup) const;
  omnisphere::types::DataTable ReadAll() const;
  omnisphere::types::DataTable Read(const omnisphere::dtos::GetItemGroup) const;
};
} // namespace omnisphere::repositories