#include <Database.hpp>
#include <DataTable.hpp>
#include <DataTable.hpp>
#include <Database.hpp>
#include <DataTable.hpp>
#include <UserModule/Repositories/UserModule.hpp>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <SQLParams.hpp>

namespace omnisphere::repositories
{
    UserModuleRepository::UserModuleRepository(std::shared_ptr<omnisphere::services::Database> Database)
        : database(Database) {}

    UserModuleRepository::~UserModuleRepository() = default;

    bool UserModuleRepository::Create(const omnisphere::dtos::CreateUserModule& userModule) const
    {
        try
        {
            const std::string query = "INSERT INTO UserModules (Entry, UserEntry, ModuleEntry, IsActive, CreatedBy, CreateDate) VALUES (?, ?, ?, ?, ?, GETDATE())";

            std::vector<omnisphere::types::SQLParam> parameters = {
                omnisphere::types::MakeSQLParam(GetCurrentSequence()),
                omnisphere::types::MakeSQLParam(userModule.UserEntry),
                omnisphere::types::MakeSQLParam(userModule.ModuleEntry),
                omnisphere::types::MakeSQLParam(userModule.IsActive ? "Y" : "N"),
                omnisphere::types::MakeSQLParam(userModule.CreatedBy)};

            if (!database->RunPrepared(query, parameters, "UserModuleRepository::Create"))
                throw std::runtime_error("[RunPrepared exception]");

            if (!UpdateSequence())
                throw std::runtime_error("[UpdateSequence exception]");

            database->CommitTransaction();

            return true;
        }
        catch (const std::exception& e)
        {
            database->RollbackTransaction();
            throw(std::runtime_error(std::string("[CreateUserModule Exception] ") + e.what()));
        }
    }

    bool UserModuleRepository::Update(const omnisphere::dtos::UpdateUserModule& userModule) const
    {
        try
        {
            std::string query = "UPDATE UserModules SET UpdateDate = GETDATE()";
            std::vector<omnisphere::types::SQLParam> parameters;

            if (userModule.IsActive.has_value())
            {
                query += ", IsActive = ?";
                parameters.push_back(omnisphere::types::MakeSQLParam(userModule.IsActive.value() ? "Y" : "N"));
            }

            query += ", LastUpdatedBy = ? WHERE Entry = ?";
            parameters.push_back(omnisphere::types::MakeSQLParam(userModule.UpdatedBy));
            parameters.push_back(omnisphere::types::MakeSQLParam(userModule.Entry));

            if (!database->RunPrepared(query, parameters, "UserModuleRepository::Update"))
                throw std::runtime_error("[RunPrepared exception]");

            database->CommitTransaction();

            return true;
        }
        catch (const std::exception& e)
        {
            database->RollbackTransaction();
            throw(std::runtime_error(std::string("[UpdateUserModule Exception] ") + e.what()));
        }
    }

    omnisphere::types::DataTable UserModuleRepository::ReadAll() const
    {
        try
        {
            const std::string query = "SELECT * FROM UserModules";

            return database->FetchResults(query, "UserModuleRepository::ReadAll");
        }
        catch (const std::exception& e)
        {
            throw(std::runtime_error(std::string("[ReadAllUserModule Exception] ") + e.what()));
        }
    }

    omnisphere::types::DataTable UserModuleRepository::Read(const omnisphere::dtos::GetUserModule& getUserModule) const
    {
        try
        {
            std::string query = "SELECT * FROM UserModules WHERE 1=1";
            std::vector<omnisphere::types::SQLParam> parameters;

            if (getUserModule.Entry.has_value())
            {
                query += " AND Entry = ?";
                parameters.push_back(omnisphere::types::MakeSQLParam(getUserModule.Entry.value()));
            }

            if (getUserModule.UserEntry.has_value())
            {
                query += " AND UserEntry = ?";
                parameters.push_back(omnisphere::types::MakeSQLParam(getUserModule.UserEntry.value()));
            }

            if (getUserModule.ModuleEntry.has_value())
            {
                query += " AND ModuleEntry = ?";
                parameters.push_back(omnisphere::types::MakeSQLParam(getUserModule.ModuleEntry.value()));
            }

            return database->FetchPrepared(query, parameters, "UserModuleRepository::Read");
        }
        catch (const std::exception& e)
        {
            throw(std::runtime_error(std::string("[ReadUserModule Exception] ") + e.what()));
        }
    }

    bool UserModuleRepository::Delete(int entry) const
    {
        try
        {
            const std::string query = "DELETE FROM UserModules WHERE Entry = ?";
            std::vector<omnisphere::types::SQLParam> parameters = {omnisphere::types::MakeSQLParam(entry)};

            if (!database->RunPrepared(query, parameters, "UserModuleRepository::Delete"))
                throw std::runtime_error("[RunPrepared exception]");

            database->CommitTransaction();

            return true;
        }
        catch (const std::exception& e)
        {
            database->RollbackTransaction();
            throw(std::runtime_error(std::string("[DeleteUserModule Exception] ") + e.what()));
        }
    }

    int UserModuleRepository::GetCurrentSequence() const
    {
        try
        {
            const std::string query = "SELECT ISNULL(UserModSequence, 0) + 1 UserModSequence FROM Sequences WHERE SeqEntry = 1";
            omnisphere::types::DataTable dataTable = database->FetchResults(query, "UserModuleRepository::GetCurrentSequence");

            return dataTable[0]["UserModSequence"];
        }
        catch (const std::exception& e)
        {
            throw(std::runtime_error(std::string("[GetCurrentSequence Exception] ") + e.what()));
        }
    }

    bool UserModuleRepository::UpdateSequence() const
    {
        try
        {
            const std::string query = "UPDATE Sequences SET UserModSequence = ISNULL(UserModSequence, 0) + 1 WHERE SeqEntry = 1";

            if (!database->RunStatement(query, "UserModuleRepository::UpdateSequence"))
                throw std::runtime_error("[RunStatement exception]");

            return true;
        }
        catch (const std::exception& e)
        {
            throw(std::runtime_error(std::string("[UpdateSequence Exception] ") + e.what()));
        }
    }
} // namespace omnisphere::repositories
