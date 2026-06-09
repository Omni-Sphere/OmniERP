#include <Database.hpp>
#include <DataTable.hpp>
#include <DataTable.hpp>
#include <Database.hpp>
#include <DataTable.hpp>
#include <UserModule/UserModule.hpp>
#include <UserModule/Repositories/UserModule.hpp>
#include <stdexcept>
#include <string>

namespace omnisphere::usermodule
{
    struct UserModule::Impl
    {
        std::shared_ptr<omnisphere::repositories::UserModuleRepository> repository;
        explicit Impl(std::shared_ptr<omnisphere::services::Database> database)
        : repository(std::make_shared<omnisphere::repositories::UserModuleRepository>(database)) {}
    };

    UserModule::UserModule(std::shared_ptr<omnisphere::services::Database> database)
    : pImpl(std::make_unique<Impl>(database)) {}
    UserModule::~UserModule() = default;

    omnisphere::models::UserModule UserModule::Add(const omnisphere::dtos::CreateUserModule& userModule) const
    {
        try
        {
            if (pImpl->repository->Create(userModule))
            {
                omnisphere::dtos::GetUserModule get;
                get.UserEntry = userModule.UserEntry;
                get.ModuleEntry = userModule.ModuleEntry;

                return Get(get);
            }
            throw std::runtime_error("Error adding user module");
        }
        catch (const std::exception& e)
        { throw std::runtime_error(std::string("[AddUserModule Exception] ") + e.what()); }
    }

    omnisphere::models::UserModule UserModule::Modify(const omnisphere::dtos::UpdateUserModule& userModule) const
    {
        try
        {
            if (pImpl->repository->Update(userModule))
            {
                omnisphere::dtos::GetUserModule get;
                get.Entry = userModule.Entry;

                return Get(get);
            }
            throw std::runtime_error("Error updating user module");
        }
        catch (const std::exception& e)
        { throw std::runtime_error(std::string("[ModifyUserModule Exception] ") + e.what()); }
    }

    std::vector<omnisphere::models::UserModule> UserModule::GetAll() const
    {
        try
        {
            std::vector<omnisphere::models::UserModule> list;
            omnisphere::types::DataTable data = pImpl->repository->ReadAll();

            for (int i = 0; i < data.RowsCount(); i++)
            {
                list.push_back(
                {data[i]["Entry"], data[i]["UserEntry"], data[i]["ModuleEntry"],
                    data[i]["IsAllowed"],
                    data[i]["CreatedBy"], data[i]["CreateDate"],
                    data[i]["LastUpdatedBy"].GetOptional<int>(),
                    data[i]["UpdateDate"].GetOptional<std::string>()});
            }

            return list;
        }
        catch (const std::exception& e)
        { throw std::runtime_error(std::string("[GetAllUserModules Exception] ") + e.what()); }
    }

    omnisphere::models::UserModule UserModule::Get(const omnisphere::dtos::GetUserModule& getUserModule) const
    {
        try
        {
            omnisphere::types::DataTable data = pImpl->repository->Read(getUserModule);

            if (data.RowsCount() == 0) throw std::runtime_error("UserModule doesn't exist");
            return
            {data[0]["Entry"], data[0]["UserEntry"], data[0]["ModuleEntry"],
                data[0]["IsAllowed"],
                data[0]["CreatedBy"], data[0]["CreateDate"],
                data[0]["LastUpdatedBy"].GetOptional<int>(),
                data[0]["UpdateDate"].GetOptional<std::string>()};
        }
        catch (const std::exception& e)
        { throw std::runtime_error(std::string("[GetUserModule Exception] ") + e.what()); }
    }

    bool UserModule::Remove(int entry) const
    {
        try
        { return pImpl->repository->Delete(entry); }
        catch (const std::exception& e)
        { throw std::runtime_error(std::string("[RemoveUserModule Exception] ") + e.what()); }
    }
} // namespace omnisphere::usermodule
