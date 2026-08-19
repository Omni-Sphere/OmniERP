#pragma once

#include <OmniData/DatabasePool.hpp>
#include <memory>
#include <vector>
#include <optional>
#include <string>

#include <OmniData/Database.hpp>
#include <OmniData/DataTable.hpp>

#include "Item/DTOs/CreateItem.hpp"
#include "Item/DTOs/ItemFilter.hpp"
#include "Item/DTOs/UpdateItem.hpp"
#include "Item/Models/Item.hpp"
#include "Item/Repositories/Item.hpp"

namespace omnisphere::services {
class Item {
public:
  explicit Item(std::shared_ptr<omnisphere::data::DatabasePool> database);

  ~Item();

  omnisphere::models::Item Get(const std::vector<std::string> &fields,
                               const omnisphere::dtos::ItemFilter &_item) const;
  std::vector<omnisphere::models::Item>
  GetAll(const std::vector<std::string> &fields) const;
  std::vector<omnisphere::models::Item>
  Search(const std::vector<std::string> &fields,
         const omnisphere::dtos::ItemFilter &_item) const;

  omnisphere::models::Item Add(const std::vector<std::string> &fields,
                               const omnisphere::dtos::CreateItem &_item) const;

  omnisphere::models::Item
  Modify(const std::vector<std::string> &fields,
         const omnisphere::dtos::UpdateItem &_item) const;

  // Batch lookup for DataLoader
  omnisphere::types::DataTable GetByIds(const std::vector<int> &ids) const;

  // Keyset pagination
  omnisphere::repositories::ItemCursorPage GetPage(std::optional<int> afterEntry, int limit) const;

private:
  struct Impl;
  std::unique_ptr<Impl> pimpl;
};
} // namespace omnisphere::services