#pragma once
#include "Permission/DTOs/Permission.hpp"
#include "Permission/Models/Permission.hpp"

namespace omnisphere::permission {
class Permission {
public:
  explicit Permission(std::shared_ptr<omnisphere::data::Database> database);
  ~Permission();

  std::vector<omnisphere::models::Permission> GetAll() const;
  omnisphere::models::Permission
  Get(const omnisphere::dtos::GetPermission &getPermission) const;

private:
  struct Impl;
  std::unique_ptr<Impl> pImpl;
};
} // namespace omnisphere::permission
