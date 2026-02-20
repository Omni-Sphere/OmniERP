#pragma once

#include <optional>
#include <string>

#include "DTOs/CreateItem.hpp"
#include "DTOs/GetItem.hpp"
#include "DTOs/SearchItems.hpp"
#include "DTOs/UpdateItem.hpp"
#include "Database.hpp"

namespace omnisphere::omnierp::repositories {
class Item {
public:
  explicit Item(
      std::shared_ptr<omnisphere::omnidata::services::Database> database);
  ~Item();

  bool Create(const omnisphere::omnierp::dtos::CreateItem &_item) const;
  bool Update(const omnisphere::omnierp::dtos::UpdateItem &_item) const;
  omnisphere::omnidata::types::DataTable
  Read(const omnisphere::omnierp::dtos::SearchItems &_item) const;
  omnisphere::omnidata::types::DataTable
  Read(const omnisphere::omnierp::dtos::GetItem &_item) const;
  omnisphere::omnidata::types::DataTable Read() const;

private:
  std::shared_ptr<omnisphere::omnidata::services::Database> Database;

  int GetCurrentSequence() const;
  bool UpdateUserSequence() const;
};
} // namespace omnisphere::omnierp::repositories