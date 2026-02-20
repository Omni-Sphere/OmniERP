#pragma once

#include <memory>
#include <vector>

#include "DTOs/CreateItemBrand.hpp"
#include "DTOs/GetItemBrand.hpp"
#include "DTOs/UpdateItemBrand.hpp"
#include "Database.hpp"
#include "Models/ItemBrand.hpp"

namespace omnisphere::omnierp::services {

class ItemBrand {
private:
  struct Impl;
  std::unique_ptr<Impl> pimpl;

public:
  explicit ItemBrand(
      std::shared_ptr<omnisphere::omnidata::services::Database> db);

  ~ItemBrand();

  bool
  Add(const omnisphere::omnierp::dtos::CreateItemBrand &createItemBrand) const;

  bool Modify(
      const omnisphere::omnierp::dtos::UpdateItemBrand &updateItemBrand) const;

  std::vector<omnisphere::omnierp::models::ItemBrand> GetAll() const;

  omnisphere::omnierp::models::ItemBrand
  Get(const omnisphere::omnierp::dtos::GetItemBrand &getItemBrand) const;
};
} // namespace omnisphere::omnierp::services