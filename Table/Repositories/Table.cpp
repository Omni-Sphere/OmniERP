#include "Repositories/Table.hpp"
#include "DataTable.hpp"
#include "Database.hpp"
#include "SQLParams.hpp"
#include <stdexcept>
#include <string>
#include <vector>

namespace omnisphere::repositories {
TableRepository::TableRepository(std::shared_ptr<omnisphere::services::Database> Database) : database(Database) {}

TableRepository::~TableRepository() = default;

bool TableRepository::Create(const omnisphere::dtos::CreateTable &table) const 
{
  try 
  {
    const std::string query = "INSERT INTO Tables (TablEntry, Code, Name, Capacity, Type, AreaEntry, FloorEntry, "
                              "CreatedBy, CreateDate) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)";

    std::vector<omnisphere::types::SQLParam> parameters = 
    {
        omnisphere::types::MakeSQLParam(GetCurrentSequence()),
        omnisphere::types::MakeSQLParam(table.Code),
        omnisphere::types::MakeSQLParam(table.Name),
        omnisphere::types::MakeSQLParam(table.Capacity),
        omnisphere::types::MakeSQLParam(table.Type),
        omnisphere::types::MakeSQLParam(table.AreaEntry),
        omnisphere::types::MakeSQLParam(table.FloorEntry),
        omnisphere::types::MakeSQLParam(table.CreatedBy),
        omnisphere::types::MakeSQLParam(table.CreateDate)
    };

    if (!database->RunPrepared(query, parameters))
      throw std::runtime_error("[RunPrepared exception]");

    if (!UpdateTableSequence())
      throw std::runtime_error("[UpdateTableSequence exception]");

    database->CommitTransaction();

    return true;
  } 
  catch (const std::exception &e) 
  {
    database->RollbackTransaction();
    throw(std::runtime_error(std::string("[Create Exception]") + " " + e.what()));
  }
}

bool TableRepository::Update(const omnisphere::dtos::UpdateTable &table) const 
{
  try 
  {
    std::string query = "UPDATE Tables SET ";
    std::vector<omnisphere::types::SQLParam> parameters;
    std::vector<std::string> setClauses;

    auto addSetParam = [&](const std::string &field, auto &value) 
    {
      if (value.has_value()) 
      {
        setClauses.push_back(field + " = ?");
        parameters.push_back(omnisphere::types::MakeSQLParam(value.value()));
      }
    };

    addSetParam("Name", table.Name);
    addSetParam("Capacity", table.Capacity);
    addSetParam("Type", table.Type);
    addSetParam("AreaEntry", table.AreaEntry);
    addSetParam("FloorEntry", table.FloorEntry);

    setClauses.push_back("LastUpdatedBy = ?");
    parameters.push_back(omnisphere::types::MakeSQLParam(table.LastUpdatedBy));

    setClauses.push_back("UpdateDate = ?");
    parameters.push_back(omnisphere::types::MakeSQLParam(table.UpdateDate));

    for (size_t i = 0; i < setClauses.size(); ++i) 
    {
      query += setClauses[i];
      if (i < setClauses.size() - 1)
        query += ", ";
    }

    query += " WHERE TablEntry = ?";
    parameters.push_back(omnisphere::types::MakeSQLParam(table.Entry));

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

omnisphere::types::DataTable TableRepository::ReadAll() const 
{
  try 
  {
    const std::string query = "SELECT TablEntry Entry, Code, Name, Capacity, Type, AreaEntry, FloorEntry, CreatedBy, CreateDate, LastUpdatedBy, UpdateDate FROM Tables";
    return database->FetchResults(query);
  } 
  catch (const std::exception &e) 
  {
    throw(std::runtime_error(std::string("[ReadAll Exception]") + " " + e.what()));
  }
}

omnisphere::types::DataTable TableRepository::Read(const omnisphere::dtos::GetTable &getTable) const 
{
  try 
  {
    std::string query = "SELECT TablEntry Entry, Code, Name, Capacity, Type, AreaEntry, FloorEntry, CreatedBy, CreateDate, LastUpdatedBy, UpdateDate FROM Tables WHERE ";
    std::vector<omnisphere::types::SQLParam> parameters;

    auto extractFilter = [&](const char *field, const auto &value) 
    {
      if (value.has_value()) 
      {
        query += std::string(field) + " = ?";
        parameters.push_back(omnisphere::types::MakeSQLParam(value.value()));
        return true;
      }
      return false;
    };

    if (!(extractFilter("TablEntry", getTable.Entry) ||
          extractFilter("Code", getTable.Code) ||
          extractFilter("AreaEntry", getTable.AreaEntry) ||
          extractFilter("FloorEntry", getTable.FloorEntry))) {
      throw std::runtime_error("GetTable: 'Entry', 'Code', 'AreaEntry' or 'FloorEntry' is required for Read");
    }

    return database->FetchPrepared(query, parameters);
  } catch (const std::exception &e) {
    throw(std::runtime_error(std::string("[Read Exception]") + " " + e.what()));
  }
}

int TableRepository::GetCurrentSequence() const 
{
  try 
  {
    const std::string query = "SELECT ISNULL(TableSequence, 0) + 1 TableSequence FROM Sequences WHERE SeqEntry = 1";
    omnisphere::types::DataTable dataTable = database->FetchResults(query);

    return dataTable[0]["TableSequence"];
  } 
  catch (const std::exception &e) 
  {
    throw(std::runtime_error(std::string("[GetCurrentSequence Exception]") + " " + e.what()));
  }
}

bool TableRepository::UpdateTableSequence() const 
{
  try 
  {
    const std::string query = "UPDATE Sequences SET TableSequence = ISNULL(TableSequence, 0) + 1";
    
    if (!database->RunStatement(query))
      throw std::runtime_error("[RunStatement exception]");

    return true;
  } catch (const std::exception &e) {
    throw(std::runtime_error(std::string("[UpdateTableSequence Exception]") + " " + e.what()));
  }
}
} // namespace omnisphere::repositories
