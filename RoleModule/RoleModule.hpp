#pragma once
#include "RoleModule/DTOs/RoleModule.hpp"
#include "RoleModule/Models/RoleModule.hpp"

namespace omnisphere::rolemodule {
class RoleModule {
public:
  explicit RoleModule(std::shared_ptr<omnisphere::data::Database> database);
  ~RoleModule();
  omnisphere::models::RoleModule
  Add(const omnisphere::dtos::CreateRoleModule &roleModule) const;
  omnisphere::models::RoleModule
  Modify(const omnisphere::dtos::UpdateRoleModule &roleModule) const;
  std::vector<omnisphere::models::RoleModule> GetAll() const;
  omnisphere::models::RoleModule
  Get(const omnisphere::dtos::GetRoleModule &getRoleModule) const;
  bool Remove(int entry) const;

private:
  struct Impl;
  std::unique_ptr<Impl> pImpl;
};
} // namespace omnisphere::rolemodule
