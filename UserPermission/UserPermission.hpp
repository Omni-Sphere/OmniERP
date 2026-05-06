#include <Database.hpp>
#include <DataTable.hpp>
#pragma once
#include <UserPermission/Models/UserPermission.hpp>
#include <UserPermission/DTOs/UserPermission.hpp>
namespace omnisphere::userpermission {
class UserPermission {
public:
    explicit UserPermission(std::shared_ptr<omnisphere::services::Database> database);
    ~UserPermission();
    omnisphere::models::UserPermission Add(const omnisphere::dtos::CreateUserPermission& userPermission) const;
    omnisphere::models::UserPermission Modify(const omnisphere::dtos::UpdateUserPermission& userPermission) const;
    std::vector<omnisphere::models::UserPermission> GetAll() const;
    omnisphere::models::UserPermission Get(const omnisphere::dtos::GetUserPermission& getUserPermission) const;
    bool Remove(int entry) const;
private:
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};
}
