#include <Database.hpp>
#include <DataTable.hpp>
#include <DataTable.hpp>
#include <Database.hpp>
#include <DataTable.hpp>
#include <Role/Role.hpp>
#include <Role/Repositories/Role.hpp>
#include <stdexcept>
#include <string>

namespace omnisphere::role
{
    struct Role::Impl
    {
        std::shared_ptr<omnisphere::repositories::RoleRepository> repository;
        explicit Impl(std::shared_ptr<omnisphere::services::Database> database)
            : repository(std::make_shared<omnisphere::repositories::RoleRepository>(database)) {}
    };

    Role::Role(std::shared_ptr<omnisphere::services::Database> database)
        : pImpl(std::make_unique<Impl>(database)) {}
    Role::~Role() = default;

    omnisphere::models::Role Role::Add(const omnisphere::dtos::CreateRole& role) const
    {
        try
        {
            if (pImpl->repository->Create(role))
            {
                omnisphere::dtos::GetRole get;
                get.Code = role.Code;

                return Get(get);
            }
            throw std::runtime_error("Error adding role");
        }
        catch (const std::exception& e)

        { throw std::runtime_error(std::string("[AddRole Exception] ") + e.what()); }
    }

    omnisphere::models::Role Role::Modify(const omnisphere::dtos::UpdateRole& role) const
    {
        try
        {
            if (pImpl->repository->Update(role))
            {
                omnisphere::dtos::GetRole get;
                get.Entry = role.Entry;

                return Get(get);
            }
            throw std::runtime_error("Error updating role");
        }
        catch (const std::exception& e)

        { throw std::runtime_error(std::string("[ModifyRole Exception] ") + e.what()); }
    }

    std::vector<omnisphere::models::Role> Role::GetAll() const
    {
        try
        {
            std::vector<omnisphere::models::Role> list;
            omnisphere::types::DataTable data = pImpl->repository->ReadAll();

            for (int i = 0; i < data.RowsCount(); i++)
            {
                list.emplace_back(data[i]["Entry"], data[i]["Code"], data[i]["Name"],
                                  data[i]["DepartmentEntry"].GetOptional<int>(),
                                  data[i]["CreatedBy"], data[i]["CreateDate"],
                                  data[i]["LastUpdatedBy"].GetOptional<int>(),
                                  data[i]["UpdateDate"].GetOptional<std::string>());
            }

            return list;
        }
        catch (const std::exception& e)

        { throw std::runtime_error(std::string("[GetAllRoles Exception] ") + e.what()); }
    }

    omnisphere::models::Role Role::Get(const omnisphere::dtos::GetRole& getRole) const
    {
        try
        {
            omnisphere::types::DataTable data = pImpl->repository->Read(getRole);

            if (data.RowsCount() == 0) throw std::runtime_error("Role doesn't exist");

            return omnisphere::models::Role(data[0]["Entry"], data[0]["Code"], data[0]["Name"],
                                            data[0]["DepartmentEntry"].GetOptional<int>(),
                                            data[0]["CreatedBy"], data[0]["CreateDate"],
                                            data[0]["LastUpdatedBy"].GetOptional<int>(),
                                            data[0]["UpdateDate"].GetOptional<std::string>());
        }
        catch (const std::exception& e)

        { throw std::runtime_error(std::string("[GetRole Exception] ") + e.what()); }
    }

    bool Role::Remove(int entry) const
    {
        try

        { return pImpl->repository->Delete(entry); }
        catch (const std::exception& e)

        { throw std::runtime_error(std::string("[RemoveRole Exception] ") + e.what()); }
    }
} // namespace omnisphere::role
