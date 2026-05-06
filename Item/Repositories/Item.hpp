#include <Database.hpp>
#include <DataTable.hpp>
#pragma once
#include <DataTable.hpp>

#include <optional>
#include <string>

#include <Item/DTOs/CreateItem.hpp>
#include <Item/DTOs/GetItem.hpp>
#include <Item/DTOs/SearchItems.hpp>
#include <Item/DTOs/UpdateItem.hpp>
#include <Database.hpp>

namespace omnisphere::repositories {
class Item {
public:
  explicit Item(std::shared_ptr<omnisphere::services::Database> database);
  ~Item();

  bool Create(const omnisphere::dtos::CreateItem &_item) const;
  bool Update(const omnisphere::dtos::UpdateItem &_item) const;
  omnisphere::types::DataTable
  Read(const omnisphere::dtos::SearchItems &_item) const;
  omnisphere::types::DataTable
  Read(const omnisphere::dtos::GetItem &_item) const;
  omnisphere::types::DataTable Read() const;

private:
  std::shared_ptr<omnisphere::services::Database> Database;

  int GetCurrentSequence() const;
  bool UpdateUserSequence() const;
};
} // namespace omnisphere::repositories