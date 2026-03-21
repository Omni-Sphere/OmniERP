#include "Floor.hpp"
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace omnisphere::repositories {
FloorRepository::FloorRepository(
    std::shared_ptr<omnisphere::services::Database> Database)
    : database(Database) {}

FloorRepository::~FloorRepository() = default;

bool FloorRepository::Create(const omnisphere::dtos::CreateFloor &floor) const 
{
    try 
    {
        const std::string query = "INSERT INTO Floors (FloorEntry, Code, Name, CreatedBy, CreateDate) VALUES (?, ?, ?, ?, ?)";

        std::vector<omnisphere::types::SQLParam> parameters = {
            omnisphere::types::MakeSQLParam(GetCurrentSequence()),
            omnisphere::types::MakeSQLParam(floor.Code),
            omnisphere::types::MakeSQLParam(floor.Name),
            omnisphere::types::MakeSQLParam(floor.CreatedBy),
            omnisphere::types::MakeSQLParam(floor.CreateDate)};

        if (!database->RunPrepared(query, parameters))
            throw std::runtime_error("[RunPrepared exception]");

        if (!UpdateFloorSequence())
            throw std::runtime_error("[UpdateFloorSequence exception]");

        database->CommitTransaction();

        return true;
    } 
    catch (const std::exception &e) 
    {
        database->RollbackTransaction();
        throw(std::runtime_error(std::string("[CreateFloor Exception]") + " " + e.what()));
  }
}

bool FloorRepository::Update(const omnisphere::dtos::UpdateFloor &floor) const {
  try {
    const std::string query = "UPDATE Floors SET Name = ?, LastUpdatedBy = ?, "
                              "UpdateDate = ? WHERE Code = ?";

    std::vector<omnisphere::types::SQLParam> parameters = {
        omnisphere::types::MakeSQLParam(floor.Name),
        omnisphere::types::MakeSQLParam(floor.LastUpdatedBy),
        omnisphere::types::MakeSQLParam(floor.UpdateDate),
        omnisphere::types::MakeSQLParam(floor.Code)};

    if (!database->RunPrepared(query, parameters))
      throw std::runtime_error("[RunPrepared exception]");

    database->CommitTransaction();

    return true;
  } catch (const std::exception &e) {
    database->RollbackTransaction();
    throw(std::runtime_error(std::string("[UpdateFloor Exception]") + " " +
                             e.what()));
  }
}

omnisphere::types::DataTable FloorRepository::ReadAll() const {
  try {
    const std::string query =
        "SELECT FloorEntry Entry, Code, Name, CreatedBy, CreateDate, LastUpdatedBy, "
        "UpdateDate FROM Floors";

    omnisphere::types::DataTable dataTable = database->FetchResults(query);

    return dataTable;
  } catch (const std::exception &e) {
    throw(std::runtime_error(std::string("[ReadAllFloor Exception]") + " " +
                             e.what()));
  }
}

omnisphere::types::DataTable
FloorRepository::Read(const omnisphere::dtos::GetFloor &getFloor) const {
  try {
    std::string query = "SELECT FloorEntry Entry, Code, Name, CreatedBy, CreateDate, "
                        "LastUpdatedBy, UpdateDate FROM Floors WHERE ";
    std::vector<omnisphere::types::SQLParam> parameters;

    if (getFloor.Entry.has_value()) {
      query += "Entry = ?";
      parameters.push_back(
          omnisphere::types::MakeSQLParam(getFloor.Entry.value()));
    } else if (getFloor.Code.has_value()) {
      query += "Code = ?";
      parameters.push_back(
          omnisphere::types::MakeSQLParam(getFloor.Code.value()));
    }

    omnisphere::types::DataTable dataTable =
        database->FetchPrepared(query, parameters);

    return dataTable;
  } catch (const std::exception &e) {
    throw(std::runtime_error(std::string("[ReadFloor Exception]") + " " +
                             e.what()));
  }
}

int FloorRepository::GetCurrentSequence() const {
  try {
    const std::string query =
        "SELECT ISNULL(FloorSequence, 0) + 1 FloorSequence FROM Sequences "
        "WHERE SeqEntry = 1";

    omnisphere::types::DataTable dataTable = database->FetchResults(query);

    return dataTable[0]["FloorSequence"];
  } catch (const std::exception &e) {
    throw(std::runtime_error(std::string("[GetCurrentSequence Exception]") +
                             " " + e.what()));
  }
}

bool FloorRepository::UpdateFloorSequence() const {
  try {
    const std::string query =
        "UPDATE Sequences SET FloorSequence = ISNULL(FloorSequence, 0) + 1";

    if (!database->RunStatement(query))
      throw std::runtime_error("[RunStatement exception]");

    return true;
  } catch (const std::exception &e) {
    throw(std::runtime_error(std::string("[UpdateFloorSequence Exception]") +
                             " " + e.what()));
  }
}
} // namespace omnisphere::repositories