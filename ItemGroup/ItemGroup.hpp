#pragma once

#include <memory>
#include <vector>

#include "DTOs/CreateItemGroup.hpp"
#include "DTOs/GetItemGroup.hpp"
#include "DTOs/UpdateItemGroup.hpp"
#include "Database/Database.hpp"
#include "Models/ItemGroup.hpp"

namespace omnicore::service {

class ItemGroup {
private:
  struct Impl;
  std::unique_ptr<Impl> pimpl;

public:
  explicit ItemGroup(std::shared_ptr<Database> db);

  ~ItemGroup();

  bool Add(const dto::CreateItemGroup &createItemGroup) const;

  bool Modify(const dto::UpdateItemGroup &updateItemGroup) const;

  std::vector<model::ItemGroup> GetAll() const;

  model::ItemGroup Get(const dto::GetItemGroup &getItemGroup) const;
};
} // namespace omnicore::service