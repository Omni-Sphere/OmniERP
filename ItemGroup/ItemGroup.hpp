#pragma once

#include <OmniData/Database.hpp>
#include <memory>
#include <vector>

#include "ItemGroup/DTOs/CreateItemGroup.hpp"
#include "ItemGroup/DTOs/GetItemGroup.hpp"
#include "ItemGroup/DTOs/UpdateItemGroup.hpp"
#include "ItemGroup/Models/ItemGroup.hpp"

namespace omnisphere::services {
class ItemGroup {
private:
  struct Impl;
  std::unique_ptr<Impl> pimpl;

public:
  explicit ItemGroup(std::shared_ptr<omnisphere::data::Database> db);

  ~ItemGroup();

  omnisphere::models::ItemGroup
  Add(const std::vector<std::string> &fields,
      const omnisphere::dtos::CreateItemGroup &createItemGroup) const;

  omnisphere::models::ItemGroup
  Modify(const std::vector<std::string> &fields,
         const omnisphere::dtos::UpdateItemGroup &updateItemGroup) const;

  std::vector<omnisphere::models::ItemGroup>
  GetAll(const std::vector<std::string> &fields = {}) const;

  omnisphere::models::ItemGroup
  Get(const std::vector<std::string> &fields,
      const omnisphere::dtos::GetItemGroup &getItemGroup) const;
};
} // namespace omnisphere::services