#pragma once
#include <OmniData/Database.hpp>
#include <RolePermission/DTOs/RolePermission.hpp>

namespace omnisphere::repositories {
class RolePermissionRepository {
public:
  explicit RolePermissionRepository(
      std::shared_ptr<omnisphere::data::Database> Database);
  ~RolePermissionRepository();

  bool
  Create(const omnisphere::dtos::CreateRolePermission &rolePermission) const;
  bool
  Update(const omnisphere::dtos::UpdateRolePermission &rolePermission) const;
  omnisphere::types::DataTable ReadAll() const;
  omnisphere::types::DataTable
  Read(const omnisphere::dtos::GetRolePermission &getRolePermission) const;
  bool Delete(int entry) const;

private:
  std::shared_ptr<omnisphere::data::Database> database;
  int GetCurrentSequence() const;
  bool UpdateSequence() const;
};
} // namespace omnisphere::repositories
