#pragma once

#include <memory>
#include <vector>

#include "Database.hpp"

#include "DTOs/CreateItem.hpp"
#include "DTOs/GetItem.hpp"
#include "DTOs/SearchItems.hpp"
#include "DTOs/UpdateItem.hpp"
#include "Models/Item.hpp"

namespace omnisphere::services {

class Item {
public:
  explicit Item(std::shared_ptr<omnisphere::services::Database> database);

  ~Item();

  omnisphere::models::Item Get(const omnisphere::dtos::GetItem &_item) const;

  std::vector<omnisphere::models::Item> GetAll() const;

  std::vector<omnisphere::models::Item>
  Search(omnisphere::dtos::SearchItems &_item) const;

  omnisphere::models::Item Add(const omnisphere::dtos::CreateItem &_item) const;

  omnisphere::models::Item
  Modify(const omnisphere::dtos::UpdateItem &_item) const;

private:
  struct Impl;
  std::unique_ptr<Impl> pimpl;
};
} // namespace omnisphere::services