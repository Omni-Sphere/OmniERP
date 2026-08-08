#pragma once
#include "Area/DTOs/CreateArea.hpp"
#include "Area/DTOs/GetArea.hpp"
#include "Area/DTOs/UpdateArea.hpp"
#include <OmniData/DataTable.hpp>
#include <OmniData/Database.hpp>
#include <memory>
#include <vector>

namespace omnisphere::repositories {
class AreaRepository {
private:
  std::shared_ptr<omnisphere::data::Database> database;

public:
  explicit AreaRepository(std::shared_ptr<omnisphere::data::Database> Database);
  ~AreaRepository();
  bool UpdateAreaSequence() const;
  int GetCurrentSequence() const;
  bool Create(const omnisphere::dtos::CreateArea &area) const;
  bool Update(const omnisphere::dtos::UpdateArea &area) const;
  omnisphere::types::DataTable ReadAll() const;
  omnisphere::types::DataTable
  Read(const omnisphere::dtos::GetArea &getArea) const;
  bool Delete(int entry) const;
};
}; // namespace omnisphere::repositories