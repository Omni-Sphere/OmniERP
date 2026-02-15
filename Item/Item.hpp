#pragma once

#include <memory>
#include <vector>

#include "Database/Database.hpp"

#include "DTOs/CreateItem.hpp"
#include "DTOs/GetItem.hpp"
#include "DTOs/SearchItems.hpp"
#include "DTOs/UpdateItem.hpp"
#include "Models/Item.hpp"

namespace omnicore::service {

class Item {
public:
  explicit Item(std::shared_ptr<Database> database);

  ~Item();

  model::Item Get(const dto::GetItem &_item) const;

  std::vector<model::Item> GetAll() const;

  std::vector<model::Item> Search(dto::SearchItems &_item) const;

  model::Item Add(const dto::CreateItem &_item) const;

  model::Item Modify(const dto::UpdateItem &_item) const;

private:
  struct Impl;
  std::unique_ptr<Impl> pimpl;
};
} // namespace omnicore::service