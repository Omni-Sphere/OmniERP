#pragma once
#include <vector>
#include <memory>

#include "DTOs/CreateTable.hpp"
#include "DTOs/GetTable.hpp"
#include "DTOs/UpdateTable.hpp"
#include "Models/Table.hpp"
#include "Database.hpp"

namespace omnisphere::table {
class Table {
  struct Impl;
  std::unique_ptr<Impl> pImpl;

public:
  explicit Table(std::shared_ptr<omnisphere::services::Database> database);
  ~Table();

  omnisphere::models::Table Add(const omnisphere::dtos::CreateTable &table) const;
  omnisphere::models::Table Modify(const omnisphere::dtos::UpdateTable &table) const;
  std::vector<omnisphere::models::Table> GetAll() const;
  omnisphere::models::Table Get(const omnisphere::dtos::GetTable &getTable) const;
  std::vector<omnisphere::models::Table> Search(const omnisphere::dtos::GetTable &getTable) const;
};
} // namespace omnisphere::table
