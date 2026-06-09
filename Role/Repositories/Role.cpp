#include <Database.hpp>
#include <DataTable.hpp>
#include <DataTable.hpp>
#include <Database.hpp>
#include <DataTable.hpp>
#include <Role/Repositories/Role.hpp>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <SQLParams.hpp>

namespace omnisphere::repositories
{
    RoleRepository::RoleRepository(std::shared_ptr<omnisphere::services::Database> Database)
        : database(Database) {}

    RoleRepository::~RoleRepository() = default;

    bool RoleRepository::Create(const omnisphere::dtos::CreateRole& role) const
    {
        try
        {
            const std::string query = "INSERT INTO Roles (Entry, Code, Name, DepartmentEntry, CreatedBy, CreateDate) VALUES (?, ?, ?, ?, ?, GETDATE())";

            std::vector<omnisphere::types::SQLParam> parameters = {
                omnisphere::types::MakeSQLParam(GetCurrentSequence()),
                omnisphere::types::MakeSQLParam(role.Code),
                omnisphere::types::MakeSQLParam(role.Name),
                omnisphere::types::MakeSQLParam(role.DepartmentEntry),
                omnisphere::types::MakeSQLParam(role.CreatedBy)};

            if (!database->RunPrepared(query, parameters, "RoleRepository::Create"))
                throw std::runtime_error("[RunPrepared exception]");

            if (!UpdateSequence())
                throw std::runtime_error("[UpdateSequence exception]");

            database->CommitTransaction();

            return true;
        }
        catch (const std::exception& e)
        {
            database->RollbackTransaction();
            throw(std::runtime_error(std::string("[CreateRole Exception] ") + e.what()));
        }
    }

    bool RoleRepository::Update(const omnisphere::dtos::UpdateRole& role) const
    {
        try
        {
            std::string query = "UPDATE Roles SET UpdateDate = GETDATE()";
            std::vector<omnisphere::types::SQLParam> parameters;

            if (role.Code.has_value())
            { query += ", Code = ?"; parameters.push_back(omnisphere::types::MakeSQLParam(role.Code.value())); }

            if (role.Name.has_value())
            { query += ", Name = ?"; parameters.push_back(omnisphere::types::MakeSQLParam(role.Name.value())); }

            if (role.DepartmentEntry.has_value())
            { query += ", DepartmentEntry = ?"; parameters.push_back(omnisphere::types::MakeSQLParam(role.DepartmentEntry.value())); }

            query += ", LastUpdatedBy = ? WHERE Entry = ?";
            parameters.push_back(omnisphere::types::MakeSQLParam(role.UpdatedBy));
            parameters.push_back(omnisphere::types::MakeSQLParam(role.Entry));

            if (!database->RunPrepared(query, parameters, "RoleRepository::Update"))
                throw std::runtime_error("[RunPrepared exception]");

            database->CommitTransaction();

            return true;
        }
        catch (const std::exception& e)
        {
            database->RollbackTransaction();
            throw(std::runtime_error(std::string("[UpdateRole Exception] ") + e.what()));
        }
    }

    omnisphere::types::DataTable RoleRepository::ReadAll() const
    {
        try
        {
            const std::string query = "SELECT * FROM Roles";

            return database->FetchResults(query, "RoleRepository::ReadAll");
        }
        catch (const std::exception& e)
        {
            throw(std::runtime_error(std::string("[ReadAllRole Exception] ") + e.what()));
        }
    }

    omnisphere::types::DataTable RoleRepository::Read(const omnisphere::dtos::GetRole& getRole) const
    {
        try
        {
            std::string query = "SELECT * FROM Roles WHERE 1=1";
            std::vector<omnisphere::types::SQLParam> parameters;

            if (getRole.Entry.has_value())
            {
                query += " AND Entry = ?";
                parameters.push_back(omnisphere::types::MakeSQLParam(getRole.Entry.value()));
            }

            if (getRole.Code.has_value())
            {
                query += " AND Code = ?";
                parameters.push_back(omnisphere::types::MakeSQLParam(getRole.Code.value()));
            }

            return database->FetchPrepared(query, parameters, "RoleRepository::Read");
        }
        catch (const std::exception& e)
        {
            throw(std::runtime_error(std::string("[ReadRole Exception] ") + e.what()));
        }
    }

    bool RoleRepository::Delete(int entry) const
    {
        try
        {
            const std::string query = "DELETE FROM Roles WHERE Entry = ?";
            std::vector<omnisphere::types::SQLParam> parameters = {omnisphere::types::MakeSQLParam(entry)};

            if (!database->RunPrepared(query, parameters, "RoleRepository::Delete"))
                throw std::runtime_error("[RunPrepared exception]");

            database->CommitTransaction();

            return true;
        }
        catch (const std::exception& e)
        {
            database->RollbackTransaction();
            throw(std::runtime_error(std::string("[DeleteRole Exception] ") + e.what()));
        }
    }

    int RoleRepository::GetCurrentSequence() const
    {
        try
        {
            const std::string query = "SELECT ISNULL(RoleSequence, 0) + 1 RoleSequence FROM Sequences WHERE SeqEntry = 1";
            omnisphere::types::DataTable dataTable = database->FetchResults(query, "RoleRepository::GetCurrentSequence");

            return dataTable[0]["RoleSequence"];
        }
        catch (const std::exception& e)
        {
            throw(std::runtime_error(std::string("[GetCurrentSequence Exception] ") + e.what()));
        }
    }

    bool RoleRepository::UpdateSequence() const
    {
        try
        {
            const std::string query = "UPDATE Sequences SET RoleSequence = ISNULL(RoleSequence, 0) + 1 WHERE SeqEntry = 1";

            if (!database->RunStatement(query, "RoleRepository::UpdateSequence"))
                throw std::runtime_error("[RunStatement exception]");

            return true;
        }
        catch (const std::exception& e)
        {
            throw(std::runtime_error(std::string("[UpdateSequence Exception] ") + e.what()));
        }
    }
} // namespace omnisphere::repositories
