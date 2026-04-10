#pragma once

#include "DTOs/CreateTable.hpp"
#include "DTOs/GetTable.hpp"
#include "DTOs/UpdateTable.hpp"
#include "Database.hpp"
#include "DataTable.hpp"
#include <memory>
#include <vector>

namespace omnisphere::repositories {
class TableRepository {
private:
  std::shared_ptr<omnisphere::services::Database> database;

public:
  explicit TableRepository(std::shared_ptr<omnisphere::services::Database> Database);
  ~TableRepository();
  bool UpdateTableSequence() const;
  int GetCurrentSequence() const;
  bool Create(const omnisphere::dtos::CreateTable &table) const;
  bool Update(const omnisphere::dtos::UpdateTable &table) const;
  omnisphere::types::DataTable ReadAll() const;
  omnisphere::types::DataTable Read(const omnisphere::dtos::GetTable &getTable) const;
  bool Delete(int entry) const;
};
};
