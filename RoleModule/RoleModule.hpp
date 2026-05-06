#include <Database.hpp>
#include <DataTable.hpp>
#pragma once
#include <RoleModule/Models/RoleModule.hpp>
#include <RoleModule/DTOs/RoleModule.hpp>
namespace omnisphere::rolemodule {
class RoleModule {
public:
    explicit RoleModule(std::shared_ptr<omnisphere::services::Database> database);
    ~RoleModule();
    omnisphere::models::RoleModule Add(const omnisphere::dtos::CreateRoleModule& roleModule) const;
    omnisphere::models::RoleModule Modify(const omnisphere::dtos::UpdateRoleModule& roleModule) const;
    std::vector<omnisphere::models::RoleModule> GetAll() const;
    omnisphere::models::RoleModule Get(const omnisphere::dtos::GetRoleModule& getRoleModule) const;
    bool Remove(int entry) const;
private:
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};
}
