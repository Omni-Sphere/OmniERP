#pragma once

#include <memory>
#include <vector>

#include "DTOs/CreateItemGroup.hpp"
#include "DTOs/GetItemGroup.hpp"
#include "DTOs/UpdateItemGroup.hpp"
#include "Database.hpp"
#include "Models/ItemGroup.hpp"

namespace omnisphere::omnierp::services {

class ItemGroup {
private:
  struct Impl;
  std::unique_ptr<Impl> pimpl;

public:
  explicit ItemGroup(
      std::shared_ptr<omnisphere::omnidata::services::Database> db);

  ~ItemGroup();

  bool
  Add(const omnisphere::omnierp::dtos::CreateItemGroup &createItemGroup) const;

  bool Modify(
      const omnisphere::omnierp::dtos::UpdateItemGroup &updateItemGroup) const;

  std::vector<omnisphere::omnierp::models::ItemGroup> GetAll() const;

  omnisphere::omnierp::models::ItemGroup
  Get(const omnisphere::omnierp::dtos::GetItemGroup &getItemGroup) const;
};
} // namespace omnisphere::omnierp::services