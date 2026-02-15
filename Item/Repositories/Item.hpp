#pragma once

#include <optional>
#include <string>


#include "DTOs/CreateItem.hpp"
#include "DTOs/GetItem.hpp"
#include "DTOs/SearchItems.hpp"
#include "DTOs/UpdateItem.hpp"
#include "Database/Database.hpp"


namespace omnicore::repository {
class Item {
public:
  explicit Item(std::shared_ptr<service::Database> database);
  ~Item();

  bool Create(const dto::CreateItem &_item) const;
  bool Update(const dto::UpdateItem &_item) const;
  type::DataTable Read(const dto::SearchItems &_item) const;
  type::DataTable Read(const dto::GetItem &_item) const;
  type::DataTable Read() const;

private:
  std::shared_ptr<service::Database> Database;

  int GetCurrentSequence() const;
  bool UpdateUserSequence() const;
};
} // namespace omnicore::repository