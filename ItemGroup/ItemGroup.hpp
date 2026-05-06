#include <Database.hpp>
#include <DataTable.hpp>
#pragma once

#include <memory>
#include <Database.hpp>
#include <vector>

#include <ItemGroup/DTOs/CreateItemGroup.hpp>
#include <ItemGroup/DTOs/GetItemGroup.hpp>
#include <ItemGroup/DTOs/UpdateItemGroup.hpp>
#include <ItemGroup/Models/ItemGroup.hpp>

namespace omnisphere::services {

class ItemGroup {
private:
  struct Impl;
  std::unique_ptr<Impl> pimpl;

public:
  explicit ItemGroup(std::shared_ptr<omnisphere::services::Database> db);

  ~ItemGroup();

  omnisphere::models::ItemGroup
  Add(const omnisphere::dtos::CreateItemGroup &createItemGroup) const;

  omnisphere::models::ItemGroup
  Modify(const omnisphere::dtos::UpdateItemGroup &updateItemGroup) const;

  std::vector<omnisphere::models::ItemGroup> GetAll() const;

  omnisphere::models::ItemGroup
  Get(const omnisphere::dtos::GetItemGroup &getItemGroup) const;
};
} // namespace omnisphere::services