#pragma once

#include <memory>
#include <vector>

#include "DTOs/CreateItemBrand.hpp"
#include "DTOs/GetItemBrand.hpp"
#include "DTOs/UpdateItemBrand.hpp"
#include "Database.hpp"
#include "Models/ItemBrand.hpp"

namespace omnisphere::services {

class ItemBrand {
private:
  struct Impl;
  std::unique_ptr<Impl> pimpl;

public:
  explicit ItemBrand(std::shared_ptr<omnisphere::services::Database> db);

  ~ItemBrand();

  omnisphere::models::ItemBrand
  Add(const omnisphere::dtos::CreateItemBrand &createItemBrand) const;

  omnisphere::models::ItemBrand
  Modify(const omnisphere::dtos::UpdateItemBrand &updateItemBrand) const;

  std::vector<omnisphere::models::ItemBrand> GetAll() const;

  omnisphere::models::ItemBrand
  Get(const omnisphere::dtos::GetItemBrand &getItemBrand) const;
};
} // namespace omnisphere::services