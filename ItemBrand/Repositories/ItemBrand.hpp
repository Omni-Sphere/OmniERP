#pragma once
#include <OmniData/DataTable.hpp>

#include "ItemBrand/DTOs/CreateItemBrand.hpp"
#include "ItemBrand/DTOs/GetItemBrand.hpp"
#include "ItemBrand/DTOs/UpdateItemBrand.hpp"
#include <OmniData/Database.hpp>

namespace omnisphere::repositories {
class ItemBrand {
private:
  std::shared_ptr<omnisphere::data::Database> database;
  int GetCurrentSequence() const;
  bool UpdateItemBrandsSequence() const;

public:
  explicit ItemBrand(std::shared_ptr<omnisphere::data::Database> Database);
  ~ItemBrand() {};
  bool Create(const omnisphere::dtos::CreateItemBrand &createItemBrand) const;
  bool Update(const omnisphere::dtos::UpdateItemBrand &updateItemBrand) const;
  omnisphere::types::DataTable
  ReadAll(const std::vector<std::string> &fields = {}) const;
  omnisphere::types::DataTable
  Search(const std::vector<std::string> &fields,
         const omnisphere::dtos::GetItemBrand &itemBrand) const;
};
} // namespace omnisphere::repositories