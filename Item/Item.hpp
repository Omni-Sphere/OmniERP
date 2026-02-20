#pragma once

#include <memory>
#include <vector>

#include "Database.hpp"

#include "DTOs/CreateItem.hpp"
#include "DTOs/GetItem.hpp"
#include "DTOs/SearchItems.hpp"
#include "DTOs/UpdateItem.hpp"
#include "Models/Item.hpp"

namespace omnisphere::omnierp::services {

class Item {
public:
  explicit Item(
      std::shared_ptr<omnisphere::omnidata::services::Database> database);

  ~Item();

  omnisphere::omnierp::models::Item
  Get(const omnisphere::omnierp::dtos::GetItem &_item) const;

  std::vector<omnisphere::omnierp::models::Item> GetAll() const;

  std::vector<omnisphere::omnierp::models::Item>
  Search(omnisphere::omnierp::dtos::SearchItems &_item) const;

  omnisphere::omnierp::models::Item
  Add(const omnisphere::omnierp::dtos::CreateItem &_item) const;

  omnisphere::omnierp::models::Item
  Modify(const omnisphere::omnierp::dtos::UpdateItem &_item) const;

private:
  struct Impl;
  std::unique_ptr<Impl> pimpl;
};
} // namespace omnisphere::omnierp::services