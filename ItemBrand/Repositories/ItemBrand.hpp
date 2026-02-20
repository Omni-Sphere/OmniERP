#pragma once

#include "DTOs/CreateItemBrand.hpp"
#include "DTOs/GetItemBrand.hpp"
#include "DTOs/UpdateItemBrand.hpp"
#include "Database.hpp"

namespace omnisphere::omnierp::repositories {
class ItemBrand {
private:
  std::shared_ptr<omnisphere::omnidata::services::Database> database;
  int GetCurrentSequence() const;
  bool UpdateItemBrandsSequence() const;

public:
  explicit ItemBrand(
      std::shared_ptr<omnisphere::omnidata::services::Database> Database);
  ~ItemBrand() {};
  bool Create(
      const omnisphere::omnierp::dtos::CreateItemBrand &createItemBrand) const;
  bool Update(
      const omnisphere::omnierp::dtos::UpdateItemBrand &updateItemBrand) const;
  omnisphere::omnidata::types::DataTable ReadAll() const;
  omnisphere::omnidata::types::DataTable
  Read(const omnisphere::omnierp::dtos::GetItemBrand) const;
};
} // namespace omnisphere::omnierp::repositories