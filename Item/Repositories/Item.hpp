#pragma once
#include <OmniData/DataTable.hpp>
#include <OmniData/DatabasePool.hpp>

#include <optional>
#include <string>
#include <vector>

#include "Item/DTOs/CreateItem.hpp"
#include "Item/DTOs/ItemFilter.hpp"
#include "Item/DTOs/UpdateItem.hpp"

namespace omnisphere::repositories {

struct ItemCursorPage {
  omnisphere::types::DataTable dataTable;
  std::optional<int> nextCursor;
  bool hasPreviousPage = false;
  int totalCount = 0;
};

class Item {
public:
  explicit Item(std::shared_ptr<omnisphere::data::DatabasePool> database);
  ~Item();

  bool Create(const omnisphere::dtos::CreateItem &_item) const;
  bool Update(const omnisphere::dtos::UpdateItem &_item) const;
  omnisphere::types::DataTable
  Search(const std::vector<std::string> &fields,
         const omnisphere::dtos::ItemFilter &_item) const;

  // Batch lookup for DataLoader
  omnisphere::types::DataTable GetByIds(const std::vector<int> &ids) const;

  // Keyset pagination
  ItemCursorPage GetPage(std::optional<int> afterEntry, int limit) const;

private:
  std::shared_ptr<omnisphere::data::DatabasePool> Database;

  int GetCurrentSequence() const;
  bool UpdateUserSequence() const;
};
} // namespace omnisphere::repositories