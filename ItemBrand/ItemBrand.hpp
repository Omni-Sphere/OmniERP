#pragma once

#include <memory>
#include <vector>

#include "DTOs/CreateItemBrand.hpp"
#include "DTOs/GetItemBrand.hpp"
#include "DTOs/UpdateItemBrand.hpp"
#include "Database/Database.hpp"
#include "Models/ItemBrand.hpp"

namespace omnicore::service {

class ItemBrand {
private:
  struct Impl;
  std::unique_ptr<Impl> pimpl;

public:
  explicit ItemBrand(std::shared_ptr<Database> db);

  ~ItemBrand();

  bool Add(const dto::CreateItemBrand &createItemBrand) const;

  bool Modify(const dto::UpdateItemBrand &updateItemBrand) const;

  std::vector<model::ItemBrand> GetAll() const;

  model::ItemBrand Get(const dto::GetItemBrand &getItemBrand) const;
};
} // namespace omnicore::service