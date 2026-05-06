#include <Database.hpp>
#include <DataTable.hpp>
#pragma once
#include <DataTable.hpp>
#include <RoleModule/DTOs/RoleModule.hpp>

namespace omnisphere::repositories {
class RoleModuleRepository {
public:
    explicit RoleModuleRepository(std::shared_ptr<omnisphere::services::Database> Database);
    ~RoleModuleRepository();

    bool Create(const omnisphere::dtos::CreateRoleModule& roleModule) const;
    bool Update(const omnisphere::dtos::UpdateRoleModule& roleModule) const;
    omnisphere::types::DataTable ReadAll() const;
    omnisphere::types::DataTable Read(const omnisphere::dtos::GetRoleModule& getRoleModule) const;
    bool Delete(int entry) const;

private:
    std::shared_ptr<omnisphere::services::Database> database;
    int GetCurrentSequence() const;
    bool UpdateSequence() const;
};
} // namespace omnisphere::repositories
