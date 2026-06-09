#include <Database.hpp>
#include <DataTable.hpp>
#include <DataTable.hpp>
#include <Database.hpp>
#include <DataTable.hpp>
#include <RolePermission/RolePermission.hpp>
#include <RolePermission/Repositories/RolePermission.hpp>
#include <stdexcept>
namespace omnisphere::rolepermission
{
    struct RolePermission::Impl
    {
        std::shared_ptr<omnisphere::repositories::RolePermissionRepository> repository;
        explicit Impl(std::shared_ptr<omnisphere::services::Database> database) : repository(std::make_shared<omnisphere::repositories::RolePermissionRepository>(database)) {}
    };
    RolePermission::RolePermission(std::shared_ptr<omnisphere::services::Database> database) : pImpl(std::make_unique<Impl>(database)) {}
    RolePermission::~RolePermission() = default;
    omnisphere::models::RolePermission RolePermission::Add(const omnisphere::dtos::CreateRolePermission& rolePermission) const
    {
        if (pImpl->repository->Create(rolePermission))
        { omnisphere::dtos::GetRolePermission get; get.RoleEntry = rolePermission.RoleEntry; get.PermissionEntry = rolePermission.PermissionEntry; return Get(get); }
        throw std::runtime_error("Error adding role permission");
    }
    omnisphere::models::RolePermission RolePermission::Modify(const omnisphere::dtos::UpdateRolePermission& rolePermission) const
    {
        if (pImpl->repository->Update(rolePermission))
        { omnisphere::dtos::GetRolePermission get; get.Entry = rolePermission.Entry; return Get(get); }
        throw std::runtime_error("Error updating role permission");
    }
    std::vector<omnisphere::models::RolePermission> RolePermission::GetAll() const
    {
        std::vector<omnisphere::models::RolePermission> list;
        omnisphere::types::DataTable data = pImpl->repository->ReadAll();

        for (int i = 0; i < data.RowsCount(); i++) list.push_back(
        {data[i]["Entry"], data[i]["RoleEntry"], data[i]["PermissionEntry"], data[i]["IsAllowed"], data[i]["CreatedBy"], data[i]["CreateDate"], data[i]["LastUpdatedBy"].GetOptional<int>(), data[i]["UpdateDate"].GetOptional<std::string>()});

        return list;
    }
    omnisphere::models::RolePermission RolePermission::Get(const omnisphere::dtos::GetRolePermission& getRolePermission) const
    {
        omnisphere::types::DataTable data = pImpl->repository->Read(getRolePermission);

        if (data.RowsCount() == 0) throw std::runtime_error("RolePermission doesn't exist");
        return
        {data[0]["Entry"], data[0]["RoleEntry"], data[0]["PermissionEntry"], data[0]["IsAllowed"], data[0]["CreatedBy"], data[0]["CreateDate"], data[0]["LastUpdatedBy"].GetOptional<int>(), data[0]["UpdateDate"].GetOptional<std::string>()};
    }
    bool RolePermission::Remove(int entry) const
    { return pImpl->repository->Delete(entry); }
}
