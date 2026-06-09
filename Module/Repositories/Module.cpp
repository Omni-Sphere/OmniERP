#include <Database.hpp>
#include <DataTable.hpp>
#include <DataTable.hpp>
#include <Database.hpp>
#include <DataTable.hpp>
#include <Module/Repositories/Module.hpp>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <SQLParams.hpp>

namespace omnisphere::repositories
{
    ModuleRepository::ModuleRepository(std::shared_ptr<omnisphere::services::Database> Database)
        : database(Database) {}

    ModuleRepository::~ModuleRepository() = default;

    omnisphere::types::DataTable ModuleRepository::ReadAll() const
    {
        try
        {
            const std::string query = "SELECT * FROM Modules";

            return database->FetchResults(query, "ModuleRepository::ReadAll");
        }
        catch (const std::exception& e)
        {
            throw(std::runtime_error(std::string("[ReadAllModule Exception] ") + e.what()));
        }
    }

    omnisphere::types::DataTable ModuleRepository::Read(const omnisphere::dtos::GetModule& getModule) const
    {
        try
        {
            std::string query = "SELECT * FROM Modules WHERE 1=1";
            std::vector<omnisphere::types::SQLParam> parameters;

            if (getModule.Entry.has_value())
            {
                query += " AND Entry = ?";
                parameters.push_back(omnisphere::types::MakeSQLParam(getModule.Entry.value()));
            }

            if (getModule.Code.has_value())
            {
                query += " AND Code = ?";
                parameters.push_back(omnisphere::types::MakeSQLParam(getModule.Code.value()));
            }

            return database->FetchPrepared(query, parameters, "ModuleRepository::Read");
        }
        catch (const std::exception& e)
        {
            throw(std::runtime_error(std::string("[ReadModule Exception] ") + e.what()));
        }
    }
} // namespace omnisphere::repositories
