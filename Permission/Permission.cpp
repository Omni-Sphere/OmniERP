#include <Database.hpp>
#include <DataTable.hpp>
#include <DataTable.hpp>
#include <Database.hpp>
#include <DataTable.hpp>
#include <Permission/Permission.hpp>
#include <Permission/Repositories/Permission.hpp>
#include <stdexcept>
#include <string>

namespace omnisphere::permission
{
    struct Permission::Impl
    {
        std::shared_ptr<omnisphere::repositories::PermissionRepository> repository;
        explicit Impl(std::shared_ptr<omnisphere::services::Database> database)
            : repository(std::make_shared<omnisphere::repositories::PermissionRepository>(database)) {}
    };

    Permission::Permission(std::shared_ptr<omnisphere::services::Database> database)
        : pImpl(std::make_unique<Impl>(database)) {}
    Permission::~Permission() = default;

    std::vector<omnisphere::models::Permission> Permission::GetAll() const
    {
        try
        {
            std::vector<omnisphere::models::Permission> list;
            omnisphere::types::DataTable data = pImpl->repository->ReadAll();

            for (int i = 0; i < data.RowsCount(); i++)
            {
                list.emplace_back(data[i]["Entry"], data[i]["Code"], data[i]["Name"],
                                  data[i]["ModuleEntry"],
                                  data[i]["CreatedBy"], data[i]["CreateDate"],
                                  data[i]["LastUpdatedBy"].GetOptional<int>(),
                                  data[i]["UpdateDate"].GetOptional<std::string>());
            }

            return list;
        }
        catch (const std::exception& e)

        { throw std::runtime_error(std::string("[GetAllPermissions Exception] ") + e.what()); }
    }

    omnisphere::models::Permission Permission::Get(const omnisphere::dtos::GetPermission& getPermission) const
    {
        try
        {
            omnisphere::types::DataTable data = pImpl->repository->Read(getPermission);

            if (data.RowsCount() == 0) throw std::runtime_error("Permission doesn't exist");

            return omnisphere::models::Permission(data[0]["Entry"], data[0]["Code"], data[0]["Name"],
                                                  data[0]["ModuleEntry"],
                                                  data[0]["CreatedBy"], data[0]["CreateDate"],
                                                  data[0]["LastUpdatedBy"].GetOptional<int>(),
                                                  data[0]["UpdateDate"].GetOptional<std::string>());
        }
        catch (const std::exception& e)

        { throw std::runtime_error(std::string("[GetPermission Exception] ") + e.what()); }
    }
} // namespace omnisphere::permission
