#include <Database.hpp>
#include <DataTable.hpp>
#include <DataTable.hpp>
#include <Database.hpp>
#include <DataTable.hpp>
#include <UserPermission/UserPermission.hpp>
#include <UserPermission/Repositories/UserPermission.hpp>
#include <stdexcept>
namespace omnisphere::userpermission
{
    struct UserPermission::Impl
    {
        std::shared_ptr<omnisphere::repositories::UserPermissionRepository> repository;
        explicit Impl(std::shared_ptr<omnisphere::services::Database> database) : repository(std::make_shared<omnisphere::repositories::UserPermissionRepository>(database)) {}
    };
    UserPermission::UserPermission(std::shared_ptr<omnisphere::services::Database> database) : pImpl(std::make_unique<Impl>(database)) {}
    UserPermission::~UserPermission() = default;
    omnisphere::models::UserPermission UserPermission::Add(const omnisphere::dtos::CreateUserPermission& userPermission) const
    {
        if (pImpl->repository->Create(userPermission))
        { omnisphere::dtos::GetUserPermission get; get.UserEntry = userPermission.UserEntry; get.PermissionEntry = userPermission.PermissionEntry; return Get(get); }
        throw std::runtime_error("Error adding user permission");
    }
    omnisphere::models::UserPermission UserPermission::Modify(const omnisphere::dtos::UpdateUserPermission& userPermission) const
    {
        if (pImpl->repository->Update(userPermission))
        { omnisphere::dtos::GetUserPermission get; get.Entry = userPermission.Entry; return Get(get); }
        throw std::runtime_error("Error updating user permission");
    }
    std::vector<omnisphere::models::UserPermission> UserPermission::GetAll() const
    {
        std::vector<omnisphere::models::UserPermission> list;
        omnisphere::types::DataTable data = pImpl->repository->ReadAll();

        for (int i = 0; i < data.RowsCount(); i++) list.push_back(

        {data[i]["Entry"], data[i]["UserEntry"], data[i]["PermissionEntry"], data[i]["IsAllowed"], data[i]["CreatedBy"], data[i]["CreateDate"], data[i]["LastUpdatedBy"].GetOptional<int>(), data[i]["UpdateDate"].GetOptional<std::string>()});

        return list;
    }
    omnisphere::models::UserPermission UserPermission::Get(const omnisphere::dtos::GetUserPermission& getUserPermission) const
    {
        omnisphere::types::DataTable data = pImpl->repository->Read(getUserPermission);

        if (data.RowsCount() == 0) throw std::runtime_error("UserPermission doesn't exist");
        return

        {data[0]["Entry"], data[0]["UserEntry"], data[0]["PermissionEntry"], data[0]["IsAllowed"], data[0]["CreatedBy"], data[0]["CreateDate"], data[0]["LastUpdatedBy"].GetOptional<int>(), data[0]["UpdateDate"].GetOptional<std::string>()};
    }
    bool UserPermission::Remove(int entry) const

    { return pImpl->repository->Delete(entry); }
}
