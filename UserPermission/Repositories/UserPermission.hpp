#include <Database.hpp>
#include <DataTable.hpp>
#pragma once
#include <DataTable.hpp>
#include <UserPermission/DTOs/UserPermission.hpp>

namespace omnisphere::repositories {
class UserPermissionRepository {
public:
    explicit UserPermissionRepository(std::shared_ptr<omnisphere::services::Database> Database);
    ~UserPermissionRepository();

    bool Create(const omnisphere::dtos::CreateUserPermission& userPermission) const;
    bool Update(const omnisphere::dtos::UpdateUserPermission& userPermission) const;
    omnisphere::types::DataTable ReadAll() const;
    omnisphere::types::DataTable Read(const omnisphere::dtos::GetUserPermission& getUserPermission) const;
    bool Delete(int entry) const;

private:
    std::shared_ptr<omnisphere::services::Database> database;
    int GetCurrentSequence() const;
    bool UpdateSequence() const;
};
} // namespace omnisphere::repositories
