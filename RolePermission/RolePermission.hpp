#include <Database.hpp>
#include <DataTable.hpp>
#pragma once
#include <RolePermission/Models/RolePermission.hpp>
#include <RolePermission/DTOs/RolePermission.hpp>
namespace omnisphere::rolepermission {
class RolePermission {
public:
    explicit RolePermission(std::shared_ptr<omnisphere::services::Database> database);
    ~RolePermission();
    omnisphere::models::RolePermission Add(const omnisphere::dtos::CreateRolePermission& rolePermission) const;
    omnisphere::models::RolePermission Modify(const omnisphere::dtos::UpdateRolePermission& rolePermission) const;
    std::vector<omnisphere::models::RolePermission> GetAll() const;
    omnisphere::models::RolePermission Get(const omnisphere::dtos::GetRolePermission& getRolePermission) const;
    bool Remove(int entry) const;
private:
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};
}
