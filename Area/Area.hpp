#pragma once
#include <vector>

#include "DTOs/CreateArea.hpp"
#include "DTOs/GetArea.hpp"
#include "DTOs/UpdateArea.hpp"
#include "Models/Area.hpp"

#include "Database.hpp"

namespace omnisphere::area {
class Area {
  struct Impl;
  std::unique_ptr<Impl> pImpl;

public:
  explicit Area(std::shared_ptr<omnisphere::services::Database> database);

  ~Area();
  omnisphere::models::Area Add(const omnisphere::dtos::CreateArea &area) const;
  omnisphere::models::Area Modify(const omnisphere::dtos::UpdateArea &area) const;
  std::vector<omnisphere::models::Area> GetAll() const;
  omnisphere::models::Area Get(const omnisphere::dtos::GetArea &getArea) const;
  std::vector<omnisphere::models::Area> Search(const omnisphere::dtos::GetArea &getArea) const;
};
} // namespace omnisphere::area
