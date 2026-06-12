#include <Database.hpp>
#include <DataTable.hpp>
#include <DataTable.hpp>
#include <Database.hpp>
#include <DataTable.hpp>
#include <UserPermission/Repositories/UserPermission.hpp>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <SQLParams.hpp>

namespace omnisphere::repositories
{
    UserPermissionRepository::UserPermissionRepository(std::shared_ptr<omnisphere::services::Database> Database)
        : database(Database) {}

    UserPermissionRepository::~UserPermissionRepository() = default;

    bool UserPermissionRepository::Create(const omnisphere::dtos::CreateUserPermission& userPermission) const
    {
        try
        {
            const std::string query = "INSERT INTO UserPermissions (Entry, UserEntry, PermissionEntry, IsActive, CreatedBy, CreateDate) VALUES (?, ?, ?, ?, ?, GETDATE())";
            std::vector<omnisphere::types::SQLParam> parameters = {
                omnisphere::types::MakeSQLParam(GetCurrentSequence()),
                omnisphere::types::MakeSQLParam(userPermission.UserEntry),
                omnisphere::types::MakeSQLParam(userPermission.PermissionEntry),
                omnisphere::types::MakeSQLParam(userPermission.IsActive ? "Y" : "N"),
                omnisphere::types::MakeSQLParam(userPermission.CreatedBy)};

            if (!database->RunPrepared(query, parameters, "UserPermissionRepository::Create")) throw std::runtime_error("[RunPrepared exception]");
            if (!UpdateSequence()) throw std::runtime_error("[UpdateSequence exception]");

            database->CommitTransaction();

            return true;
        }
        catch (const std::exception& e)

        { database->RollbackTransaction(); throw(std::runtime_error(std::string("[CreateUserPermission Exception] ") + e.what())); }
    }

    bool UserPermissionRepository::Update(const omnisphere::dtos::UpdateUserPermission& userPermission) const
    {
        try
        {
            std::string query = "UPDATE UserPermissions SET UpdateDate = GETDATE()";
            std::vector<omnisphere::types::SQLParam> parameters;

            if (userPermission.IsActive.has_value())
            { query += ", IsActive = ?"; parameters.push_back(omnisphere::types::MakeSQLParam(userPermission.IsActive.value() ? "Y" : "N")); }
            query += ", LastUpdatedBy = ? WHERE Entry = ?";
            parameters.push_back(omnisphere::types::MakeSQLParam(userPermission.UpdatedBy));
            parameters.push_back(omnisphere::types::MakeSQLParam(userPermission.Entry));

            if (!database->RunPrepared(query, parameters, "UserPermissionRepository::Update")) throw std::runtime_error("[RunPrepared exception]");

            database->CommitTransaction();

            return true;
        }
        catch (const std::exception& e)

        { database->RollbackTransaction(); throw(std::runtime_error(std::string("[UpdateUserPermission Exception] ") + e.what())); }
    }

    omnisphere::types::DataTable UserPermissionRepository::ReadAll() const
    {
        try

        { return database->FetchResults("SELECT * FROM UserPermissions", "UserPermissionRepository::ReadAll"); }
        catch (const std::exception& e)

        { throw(std::runtime_error(std::string("[ReadAllUserPermission Exception] ") + e.what())); }
    }

    omnisphere::types::DataTable UserPermissionRepository::Read(const omnisphere::dtos::GetUserPermission& getUserPermission) const
    {
        try
        {
            std::string query = "SELECT * FROM UserPermissions WHERE 1=1";
            std::vector<omnisphere::types::SQLParam> parameters;

            if (getUserPermission.Entry.has_value())
            { query += " AND Entry = ?"; parameters.push_back(omnisphere::types::MakeSQLParam(getUserPermission.Entry.value())); }

            if (getUserPermission.UserEntry.has_value())
            { query += " AND UserEntry = ?"; parameters.push_back(omnisphere::types::MakeSQLParam(getUserPermission.UserEntry.value())); }

            if (getUserPermission.PermissionEntry.has_value())
            { query += " AND PermissionEntry = ?"; parameters.push_back(omnisphere::types::MakeSQLParam(getUserPermission.PermissionEntry.value())); }

            return database->FetchPrepared(query, parameters, "UserPermissionRepository::Read");
        }
        catch (const std::exception& e)

        { throw(std::runtime_error(std::string("[ReadUserPermission Exception] ") + e.what())); }
    }

    bool UserPermissionRepository::Delete(int entry) const
    {
        try
        {
            std::vector<omnisphere::types::SQLParam> parameters = {omnisphere::types::MakeSQLParam(entry)};

            if (!database->RunPrepared("DELETE FROM UserPermissions WHERE Entry = ?", parameters, "UserPermissionRepository::Delete")) throw std::runtime_error("[RunPrepared exception]");

            database->CommitTransaction();

            return true;
        }
        catch (const std::exception& e)

        { database->RollbackTransaction(); throw(std::runtime_error(std::string("[DeleteUserPermission Exception] ") + e.what())); }
    }

    int UserPermissionRepository::GetCurrentSequence() const
    {
        try
        {
            omnisphere::types::DataTable dataTable = database->FetchResults("SELECT ISNULL(UserPermSequence, 0) + 1 UserPermSequence FROM Sequences WHERE Entry = 1");

            return dataTable[0]["UserPermSequence"];
        }
        catch (const std::exception& e)

        { throw(std::runtime_error(std::string("[GetCurrentSequence Exception] ") + e.what())); }
    }

    bool UserPermissionRepository::UpdateSequence() const
    {
        try
        {
            if (!database->RunStatement("UPDATE Sequences SET UserPermSequence = ISNULL(UserPermSequence, 0) + 1 WHERE Entry = 1")) throw std::runtime_error("[RunStatement exception]");

            return true;
        }
        catch (const std::exception& e)

        { throw(std::runtime_error(std::string("[UpdateSequence Exception] ") + e.what())); }
    }
} // namespace omnisphere::repositories
