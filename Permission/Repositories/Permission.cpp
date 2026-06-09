#include <Database.hpp>
#include <DataTable.hpp>
#include <DataTable.hpp>
#include <Database.hpp>
#include <DataTable.hpp>
#include <Permission/Repositories/Permission.hpp>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <SQLParams.hpp>

namespace omnisphere::repositories
{
    PermissionRepository::PermissionRepository(std::shared_ptr<omnisphere::services::Database> Database)
    : database(Database) {}

    PermissionRepository::~PermissionRepository() = default;

    omnisphere::types::DataTable PermissionRepository::ReadAll() const
    {
        try
        {
            const std::string query = "SELECT * FROM Permissions";

            return database->FetchResults(query, "PermissionRepository::ReadAll");
        }
        catch (const std::exception& e)
        {
            throw(std::runtime_error(std::string("[ReadAllPermission Exception] ") + e.what()));
        }
    }

    omnisphere::types::DataTable PermissionRepository::Read(const omnisphere::dtos::GetPermission& getPermission) const
    {
        try
        {
            std::string query = "SELECT * FROM Permissions WHERE 1=1";
            std::vector<omnisphere::types::SQLParam> parameters;

            if (getPermission.Entry.has_value())
            {
                query += " AND Entry = ?";
                parameters.push_back(omnisphere::types::MakeSQLParam(getPermission.Entry.value()));
            }

            if (getPermission.Code.has_value())
            {
                query += " AND Code = ?";
                parameters.push_back(omnisphere::types::MakeSQLParam(getPermission.Code.value()));
            }

            if (getPermission.ModuleEntry.has_value())
            {
                query += " AND ModuleEntry = ?";
                parameters.push_back(omnisphere::types::MakeSQLParam(getPermission.ModuleEntry.value()));
            }

            return database->FetchPrepared(query, parameters, "PermissionRepository::Read");
        }
        catch (const std::exception& e)
        {
            throw(std::runtime_error(std::string("[ReadPermission Exception] ") + e.what()));
        }
    }
} // namespace omnisphere::repositories
