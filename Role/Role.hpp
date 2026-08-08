#pragma once
#include "Role/DTOs/Role.hpp"
#include "Role/Models/Role.hpp"

namespace omnisphere::role {
class Role {
public:
  explicit Role(std::shared_ptr<omnisphere::data::Database> database);
  ~Role();

  omnisphere::models::Role Add(const omnisphere::dtos::CreateRole &role) const;
  omnisphere::models::Role
  Modify(const omnisphere::dtos::UpdateRole &role) const;
  std::vector<omnisphere::models::Role> GetAll() const;
  omnisphere::models::Role Get(const omnisphere::dtos::GetRole &getRole) const;
  bool Remove(int entry) const;

private:
  struct Impl;
  std::unique_ptr<Impl> pImpl;
};
} // namespace omnisphere::role
