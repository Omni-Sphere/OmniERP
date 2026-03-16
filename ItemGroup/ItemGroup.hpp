#pragma once

#include <memory>
#include <vector>

#include "DTOs/CreateItemGroup.hpp"
#include "DTOs/GetItemGroup.hpp"
#include "DTOs/UpdateItemGroup.hpp"
#include "Database.hpp"
#include "Models/ItemGroup.hpp"

namespace omnisphere::services {

class ItemGroup {
private:
  struct Impl;
  std::unique_ptr<Impl> pimpl;

public:
  explicit ItemGroup(std::shared_ptr<omnisphere::services::Database> db);

  ~ItemGroup();

  bool Add(const omnisphere::dtos::CreateItemGroup &createItemGroup) const;

  bool Modify(const omnisphere::dtos::UpdateItemGroup &updateItemGroup) const;

  std::vector<omnisphere::models::ItemGroup> GetAll() const;

  omnisphere::models::ItemGroup
  Get(const omnisphere::dtos::GetItemGroup &getItemGroup) const;
};
} // namespace omnisphere::services