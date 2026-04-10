#pragma once
#include <vector>

#include "DTOs/CreateFloor.hpp"
#include "DTOs/GetFloor.hpp"
#include "DTOs/UpdateFloor.hpp"
#include "Models/Floor.hpp"

#include "Database.hpp"

namespace omnisphere::floor {
class Floor {
  struct Impl;
  std::unique_ptr<Impl> pImpl;

public:
  explicit Floor(std::shared_ptr<omnisphere::services::Database> database);

  ~Floor();
  omnisphere::models::Floor Add(const omnisphere::dtos::CreateFloor &floor) const;
  omnisphere::models::Floor Modify(const omnisphere::dtos::UpdateFloor &floor) const;
  std::vector<omnisphere::models::Floor> GetAll() const;
  omnisphere::models::Floor Get(const omnisphere::dtos::GetFloor &getFloor) const;
  bool Remove(int entry) const;
};
} // namespace omnisphere::floor
