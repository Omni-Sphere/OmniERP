#include <Database.hpp>
#include <DataTable.hpp>
#pragma once
#include <DataTable.hpp>

#include <Area/DTOs/CreateArea.hpp>
#include <Area/DTOs/GetArea.hpp>
#include <Area/DTOs/UpdateArea.hpp>
#include <Database.hpp>
#include <memory>
#include <Database.hpp>
#include <vector>

namespace omnisphere::repositories {
class AreaRepository {
private:
  std::shared_ptr<omnisphere::services::Database> database;

public:
  explicit AreaRepository(std::shared_ptr<omnisphere::services::Database> Database);
  ~AreaRepository();
  bool UpdateAreaSequence() const;
  int GetCurrentSequence() const;
  bool Create(const omnisphere::dtos::CreateArea &area) const;
  bool Update(const omnisphere::dtos::UpdateArea &area) const;
  omnisphere::types::DataTable ReadAll() const;
  omnisphere::types::DataTable Read(const omnisphere::dtos::GetArea &getArea) const;
  bool Delete(int entry) const;
};
};