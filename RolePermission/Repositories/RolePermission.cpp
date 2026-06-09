#include <Database.hpp>
#include <DataTable.hpp>
#include <DataTable.hpp>
#include <Database.hpp>
#include <DataTable.hpp>
#include <RolePermission/Repositories/RolePermission.hpp>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <SQLParams.hpp>

namespace omnisphere::repositories
{
    RolePermissionRepository::RolePermissionRepository(std::shared_ptr<omnisphere::services::Database> Database)
    : database(Database) {}

    RolePermissionRepository::~RolePermissionRepository() = default;

    bool RolePermissionRepository::Create(const omnisphere::dtos::CreateRolePermission& rolePermission) const
    {
        try
        {
            const std::string query = "INSERT INTO RolePermissions (Entry, RoleEntry, PermissionEntry, IsAllowed, CreatedBy, CreateDate) VALUES (?, ?, ?, ?, ?, GETDATE())";
            std::vector<omnisphere::types::SQLParam> parameters = {
                omnisphere::types::MakeSQLParam(GetCurrentSequence()),
                omnisphere::types::MakeSQLParam(rolePermission.RoleEntry),
                omnisphere::types::MakeSQLParam(rolePermission.PermissionEntry),
                omnisphere::types::MakeSQLParam(rolePermission.IsAllowed ? "Y" : "N"),
                omnisphere::types::MakeSQLParam(rolePermission.CreatedBy)};

            if (!database->RunPrepared(query, parameters, "RolePermissionRepository::Create")) throw std::runtime_error("[RunPrepared exception]");
            if (!UpdateSequence()) throw std::runtime_error("[UpdateSequence exception]");

            database->CommitTransaction();

            return true;
        }
        catch (const std::exception& e)
        { database->RollbackTransaction(); throw(std::runtime_error(std::string("[CreateRolePermission Exception] ") + e.what())); }
    }

    bool RolePermissionRepository::Update(const omnisphere::dtos::UpdateRolePermission& rolePermission) const
    {
        try
        {
            std::string query = "UPDATE RolePermissions SET UpdateDate = GETDATE()";
            std::vector<omnisphere::types::SQLParam> parameters;

            if (rolePermission.IsAllowed.has_value())
            { query += ", IsAllowed = ?"; parameters.push_back(omnisphere::types::MakeSQLParam(rolePermission.IsAllowed.value() ? "Y" : "N")); }
            query += ", LastUpdatedBy = ? WHERE Entry = ?";
            parameters.push_back(omnisphere::types::MakeSQLParam(rolePermission.UpdatedBy));
            parameters.push_back(omnisphere::types::MakeSQLParam(rolePermission.Entry));

            if (!database->RunPrepared(query, parameters, "RolePermissionRepository::Update")) throw std::runtime_error("[RunPrepared exception]");

            database->CommitTransaction();

            return true;
        }
        catch (const std::exception& e)
        { database->RollbackTransaction(); throw(std::runtime_error(std::string("[UpdateRolePermission Exception] ") + e.what())); }
    }

    omnisphere::types::DataTable RolePermissionRepository::ReadAll() const
    {
        try
        { return database->FetchResults("SELECT * FROM RolePermissions", "RolePermissionRepository::ReadAll"); }
        catch (const std::exception& e)
        { throw(std::runtime_error(std::string("[ReadAllRolePermission Exception] ") + e.what())); }
    }

    omnisphere::types::DataTable RolePermissionRepository::Read(const omnisphere::dtos::GetRolePermission& getRolePermission) const
    {
        try
        {
            std::string query = "SELECT * FROM RolePermissions WHERE 1=1";
            std::vector<omnisphere::types::SQLParam> parameters;

            if (getRolePermission.Entry.has_value())
            { query += " AND Entry = ?"; parameters.push_back(omnisphere::types::MakeSQLParam(getRolePermission.Entry.value())); }

            if (getRolePermission.RoleEntry.has_value())
            { query += " AND RoleEntry = ?"; parameters.push_back(omnisphere::types::MakeSQLParam(getRolePermission.RoleEntry.value())); }

            if (getRolePermission.PermissionEntry.has_value())
            { query += " AND PermissionEntry = ?"; parameters.push_back(omnisphere::types::MakeSQLParam(getRolePermission.PermissionEntry.value())); }

            return database->FetchPrepared(query, parameters, "RolePermissionRepository::Read");
        }
        catch (const std::exception& e)
        { throw(std::runtime_error(std::string("[ReadRolePermission Exception] ") + e.what())); }
    }

    bool RolePermissionRepository::Delete(int entry) const
    {
        try
        {
            std::vector<omnisphere::types::SQLParam> parameters = {omnisphere::types::MakeSQLParam(entry)};

            if (!database->RunPrepared("DELETE FROM RolePermissions WHERE Entry = ?", parameters, "RolePermissionRepository::Delete")) throw std::runtime_error("[RunPrepared exception]");

            database->CommitTransaction();

            return true;
        }
        catch (const std::exception& e)
        { database->RollbackTransaction(); throw(std::runtime_error(std::string("[DeleteRolePermission Exception] ") + e.what())); }
    }

    int RolePermissionRepository::GetCurrentSequence() const
    {
        try
        {
            omnisphere::types::DataTable dataTable = database->FetchResults("SELECT ISNULL(RolePermSequence, 0) + 1 RolePermSequence FROM Sequences WHERE SeqEntry = 1");

            return dataTable[0]["RolePermSequence"];
        }
        catch (const std::exception& e)
        { throw(std::runtime_error(std::string("[GetCurrentSequence Exception] ") + e.what())); }
    }

    bool RolePermissionRepository::UpdateSequence() const
    {
        try
        {
            if (!database->RunStatement("UPDATE Sequences SET RolePermSequence = ISNULL(RolePermSequence, 0) + 1 WHERE SeqEntry = 1")) throw std::runtime_error("[RunStatement exception]");

            return true;
        }
        catch (const std::exception& e)
        { throw(std::runtime_error(std::string("[UpdateSequence Exception] ") + e.what())); }
    }
} // namespace omnisphere::repositories
