#pragma once
#include <OmniData/DataTable.hpp>

#include <optional>
#include <string>

#include "Item/DTOs/CreateItem.hpp"
#include "Item/DTOs/ItemFilter.hpp"
#include "Item/DTOs/UpdateItem.hpp"
#include <OmniData/Database.hpp>
#include <string>
#include <vector>

namespace omnisphere::repositories {
class Item {
public:
  explicit Item(std::shared_ptr<omnisphere::data::Database> database);
  ~Item();

  bool Create(const omnisphere::dtos::CreateItem &_item) const;
  bool Update(const omnisphere::dtos::UpdateItem &_item) const;
  omnisphere::types::DataTable
  Search(const std::vector<std::string> &fields,
         const omnisphere::dtos::ItemFilter &_item) const;
  // Full read without field filtering (SELECT *)

private:
  std::shared_ptr<omnisphere::data::Database> Database;

  int GetCurrentSequence() const;
  bool UpdateUserSequence() const;
};
} // namespace omnisphere::repositories