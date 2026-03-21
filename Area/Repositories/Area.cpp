#include "Repositories/Area.hpp"
#include "DataTable.hpp"
#include "Database.hpp"
#include "SQLParams.hpp"
#include "DTOs/CreateArea.hpp"
#include "DTOs/UpdateArea.hpp"
#include "Models/Area.hpp"
#include <stdexcept>
#include <string>
#include <vector>

namespace omnisphere::repositories 
{
    AreaRepository::AreaRepository(std::shared_ptr<omnisphere::services::Database> Database) : database(Database) {}

    AreaRepository::~AreaRepository() {}

    bool AreaRepository::Create(const omnisphere::dtos::CreateArea &area) const {
        try 
        {
            const std::string query = "INSERT INTO Areas (AreaEntry, Code, Name, Color, Icon, Capacity, FloorEntry, CreatedBy, CreateDate) VALUES (?, ?, ?, ?, ?, ?, ?, ?)";

            std::vector<omnisphere::types::SQLParam> parameters = {
                omnisphere::types::MakeSQLParam(GetCurrentSequence()),
                omnisphere::types::MakeSQLParam(area.Code),
                omnisphere::types::MakeSQLParam(area.Name),
                omnisphere::types::MakeSQLParam(area.Color),
                omnisphere::types::MakeSQLParam(area.Icon),
                omnisphere::types::MakeSQLParam(area.Capacity),
                omnisphere::types::MakeSQLParam(area.FloorEntry),
                omnisphere::types::MakeSQLParam(area.CreatedBy),
                omnisphere::types::MakeSQLParam(area.CreateDate)
            };

            if (!database->RunPrepared(query, parameters))
                throw std::runtime_error("[RunPrepared exception]");

            if (!UpdateAreaSequence())
                throw std::runtime_error("[UpdateAreaSequence exception]");

            database->CommitTransaction();

            return true;
        } 
        catch (const std::exception &e) 
        {
            database->RollbackTransaction();
            throw(std::runtime_error(std::string("[Create Exception]") + " " + e.what()));
        }
    }

    bool AreaRepository::Update(const omnisphere::dtos::UpdateArea &area) const {
        try 
        {
            std::string query = "UPDATE Areas SET ";
            std::vector<omnisphere::types::SQLParam> parameters;
            std::vector<std::string> setClauses;

            if(area.Name.has_value()) {
                setClauses.push_back("Name = ?");
                parameters.push_back(omnisphere::types::MakeSQLParam(area.Name.value()));
            }
            if(area.Color.has_value()) {
                setClauses.push_back("Color = ?");
                parameters.push_back(omnisphere::types::MakeSQLParam(area.Color.value()));
            }
            if(area.Capacity.has_value()) {
                setClauses.push_back("Capacity = ?");
                parameters.push_back(omnisphere::types::MakeSQLParam(area.Capacity.value()));
            }
            if(area.FloorEntry.has_value()) {
                setClauses.push_back("FloorEntry = ?");
                parameters.push_back(omnisphere::types::MakeSQLParam(area.FloorEntry.value()));
            }
            if(area.Icon.has_value()) {
                setClauses.push_back("Icon = ?");
                parameters.push_back(omnisphere::types::MakeSQLParam(area.Icon.value()));
            }
            setClauses.push_back("LastUpdatedBy = ?");
            parameters.push_back(omnisphere::types::MakeSQLParam(area.LastUpdatedBy));
            
            setClauses.push_back("UpdateDate = ?");
            parameters.push_back(omnisphere::types::MakeSQLParam(area.UpdateDate));

            if (setClauses.empty()) return true;

            for (size_t i = 0; i < setClauses.size(); ++i) {
                query += setClauses[i];
                if (i < setClauses.size() - 1) query += ", ";
            }

            query += " WHERE AreaEntry = ?";
            if (area.Entry.has_value()) {
                parameters.push_back(omnisphere::types::MakeSQLParam(area.Entry.value()));
            } else {
                throw std::runtime_error("UpdateArea: 'Entry' is required for UPDATE");
            }

            if (!database->RunPrepared(query, parameters))
                throw std::runtime_error("[RunPrepared exception]");

            database->CommitTransaction();

            return true;
        } 
        catch (const std::exception &e) 
        {
            database->RollbackTransaction();
            throw(std::runtime_error(std::string("[Update Exception]") + " " + e.what()));
        }
    }

    omnisphere::types::DataTable AreaRepository::ReadAll() const {
        try 
        {
            const std::string query = "SELECT AreaEntry Entry, Code, Name, Color, Icon, Capacity, FloorEntry, CreatedBy, CreateDate, LastUpdatedBy, UpdateDate FROM Areas";

            omnisphere::types::DataTable dataTable = database->FetchResults(query);

            return dataTable;
        } 
        catch (const std::exception &e) 
        {
            throw(std::runtime_error(std::string("[ReadAll Exception]") + " " + e.what()));
        }
    }

    omnisphere::types::DataTable AreaRepository::Read(const omnisphere::dtos::GetArea &getArea) const {
        try 
        {
            std::string query = "SELECT AreaEntry Entry, Code, Name, Color, Icon, Capacity, FloorEntry, CreatedBy, CreateDate, LastUpdatedBy, UpdateDate FROM Areas WHERE ";
            std::vector<omnisphere::types::SQLParam> parameters;

            if (getArea.Entry.has_value()) {
                query += "AreaEntry = ?";
                parameters.push_back(omnisphere::types::MakeSQLParam(getArea.Entry.value()));
            } else if (getArea.Code.has_value()) {
                query += "Code = ?";
                parameters.push_back(omnisphere::types::MakeSQLParam(getArea.Code.value()));
            } else if (getArea.FloorEntry.has_value()) {
                query += "FloorEntry = ?";
                parameters.push_back(omnisphere::types::MakeSQLParam(getArea.FloorEntry.value()));
            } else {
                throw std::runtime_error("GetArea: 'Entry', 'Code' or 'FloorEntry' is required for Read");
            }

            return database->FetchPrepared(query, parameters);
        } 
        catch (const std::exception &e) 
        {
            throw(std::runtime_error(std::string("[Read Exception]") + " " + e.what()));
        }
    }

    int AreaRepository::GetCurrentSequence() const {
        try 
        {
            const std::string query = "SELECT ISNULL(AreaSequence, 0) + 1 AreaSequence FROM Sequences WHERE SeqEntry = 1";

            omnisphere::types::DataTable dataTable = database->FetchResults(query);

            return dataTable[0]["AreaSequence"];
        } 
        catch (const std::exception &e) 
        {
            throw(std::runtime_error(std::string("[GetCurrentSequence Exception]") + " " + e.what()));
        }
    }

    bool AreaRepository::UpdateAreaSequence() const {
        try 
        {
            const std::string query = "UPDATE Sequences SET AreaSequence = ISNULL(AreaSequence, 0) + 1";

            if (!database->RunStatement(query))
                throw std::runtime_error("[RunStatement exception]");

            return true;
        } 
        catch (const std::exception &e) 
        {
            throw(std::runtime_error(std::string("[UpdateAreaSequence Exception]") + " " + e.what()));
        }
    }
}