#include "Floor/Repositories/Floor.hpp"
#include "Floor/Models/Floor.hpp"
#include <OmniData/DataTable.hpp>
#include <OmniData/Database.hpp>
#include <OmniData/QueryBuilder.hpp>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace omnisphere::repositories {
FloorRepository::FloorRepository(
    std::shared_ptr<omnisphere::data::Database> Database)
    : database(Database) {}

FloorRepository::~FloorRepository() = default;

bool FloorRepository::Create(const omnisphere::dtos::CreateFloor &floor) const {
  try {
    const std::string query =
        "INSERT INTO \"Floors\" (\"Entry\", \"Code\", \"Name\", \"CreatedBy\", \"CreateDate\", "
        "\"IsActive\") VALUES (?, ?, ?, ?, ?, true)";

    std::vector<omnisphere::types::SQLParam> parameters = {
        omnisphere::types::MakeSQLParam(GetCurrentSequence()),
        omnisphere::types::MakeSQLParam(floor.Code),
        omnisphere::types::MakeSQLParam(floor.Name),
        omnisphere::types::MakeSQLParam(floor.CreatedBy),
        omnisphere::types::MakeSQLParam(floor.CreateDate)};

    if (!database->RunPrepared(query, parameters, "FloorRepository::Create"))
      throw std::runtime_error("[RunPrepared exception]");

    if (!UpdateFloorSequence())
      throw std::runtime_error("[UpdateFloorSequence exception]");

    database->CommitTransaction();

    return true;
  } catch (const std::exception &e) {
    database->RollbackTransaction();
    throw(std::runtime_error(std::string("[CreateFloor Exception]") + " " +
                             e.what()));
  }
}

bool FloorRepository::Update(const omnisphere::dtos::UpdateFloor &floor) const {
  try {
    const std::string query = "UPDATE \"Floors\" SET \"Name\" = ?, \"LastUpdatedBy\" = ?, "
                              "\"UpdateDate\" = ? WHERE \"Code\" = ?";

    std::vector<omnisphere::types::SQLParam> parameters = {
        omnisphere::types::MakeSQLParam(floor.Name),
        omnisphere::types::MakeSQLParam(floor.LastUpdatedBy),
        omnisphere::types::MakeSQLParam(floor.UpdateDate),
        omnisphere::types::MakeSQLParam(floor.Code)};

    if (!database->RunPrepared(query, parameters, "FloorRepository::Update"))
      throw std::runtime_error("[RunPrepared exception]");

    database->CommitTransaction();

    return true;
  } catch (const std::exception &e) {
    database->RollbackTransaction();
    throw(std::runtime_error(std::string("[UpdateFloor Exception]") + " " +
                             e.what()));
  }
}

omnisphere::types::DataTable FloorRepository::ReadAll(const std::vector<std::string>& fields) const {
  try {
    auto selectFields = omnisphere::types::FilterModelFields<omnisphere::models::Floor>(fields);
    std::vector<omnisphere::types::Condition> conditions = {{"", "\"IsActive\"", "=", "true"}};
    auto qp = omnisphere::types::BuildQueryParts(selectFields, conditions);
    std::string query = "SELECT " + qp.SelectClause + " FROM \"Floors\" WHERE " + qp.WhereClause;

    return database->FetchResults(query, "FloorRepository::ReadAll");
  } catch (const std::exception &e) {
    throw(std::runtime_error(std::string("[ReadAllFloor Exception] ") + " " +
                             e.what()));
  }
}

omnisphere::types::DataTable
FloorRepository::Read(const omnisphere::dtos::GetFloor &getFloor, const std::vector<std::string>& fields) const {
  try {
    auto selectFields = omnisphere::types::FilterModelFields<omnisphere::models::Floor>(fields);
    std::vector<omnisphere::types::Condition> conditions = {{"", "\"IsActive\"", "=", "true"}};
    std::vector<omnisphere::types::SQLParam> parameters;

    if (getFloor.Entry.has_value()) {
      conditions.push_back({"", "\"Entry\"", "=", "?"});
      parameters.push_back(
          omnisphere::types::MakeSQLParam(getFloor.Entry.value()));
    } else if (getFloor.Code.has_value()) {
      conditions.push_back({"", "\"Code\"", "=", "?"});
      parameters.push_back(
          omnisphere::types::MakeSQLParam(getFloor.Code.value()));
    }

    auto qp = omnisphere::types::BuildQueryParts(selectFields, conditions);
    std::string query = "SELECT " + qp.SelectClause + " FROM \"Floors\" WHERE " + qp.WhereClause;

    return database->FetchPrepared(query, parameters, "FloorRepository::Read");
  } catch (const std::exception &e) {
    throw(std::runtime_error(std::string("[ReadFloor Exception]") + " " +
                             e.what()));
  }
}

int FloorRepository::GetCurrentSequence() const {
  try {
    const std::string query =
        "SELECT COALESCE(\"FloorSequence\", 0) + 1 \"FloorSequence\" FROM \"Sequences\" "
        "WHERE \"Entry\" = 1";

    omnisphere::types::DataTable dataTable =
        database->FetchResults(query, "FloorRepository::GetCurrentSequence");

    if (dataTable.IsEmpty()) {
      // Initialize Sequences table with default row
      database->RunStatement(
          "INSERT INTO \"Sequences\" (\"Entry\", \"Code\", \"Name\", \"UserSequence\", \"CreatedBy\", "
          "\"CreateDate\", \"StoreSequence\", \"CustomerSequence\", \"AreaSequence\", "
          "\"FloorSequence\", \"DeparmentSequence\") VALUES (1, 'SEQ001', 'Sequence "
          "Manager', 1, 1, CURRENT_TIMESTAMP, NULL, NULL, NULL, NULL, NULL) ON CONFLICT (\"Entry\") DO NOTHING",
          "FloorRepository::InitializeSequences");
      dataTable = database->FetchResults(
          query, "FloorRepository::GetCurrentSequence-Retry");
      if (dataTable.IsEmpty())
        return 1;
    }

    return dataTable[0]["FloorSequence"];
  } catch (const std::exception &e) {
    throw(std::runtime_error(std::string("[GetCurrentSequence Exception]") +
                             " " + e.what()));
  }
}

bool FloorRepository::UpdateFloorSequence() const {
  try {
    const std::string query =
        "UPDATE \"Sequences\" SET \"FloorSequence\" = COALESCE(\"FloorSequence\", 0) + 1 WHERE \"Entry\" = 1";

    if (!database->RunStatement(query, "FloorRepository::UpdateFloorSequence"))
      throw std::runtime_error("[RunStatement exception]");

    return true;
  } catch (const std::exception &e) {
    throw(std::runtime_error(std::string("[UpdateFloorSequence Exception]") +
                             " " + e.what()));
  }
}

bool FloorRepository::Delete(int entry) const {
  try {
    const std::string query =
        "UPDATE \"Floors\" SET \"IsActive\" = false WHERE \"Entry\" = ?";
    std::vector<omnisphere::types::SQLParam> parameters = {
        omnisphere::types::MakeSQLParam(entry)};

    if (!database->RunPrepared(query, parameters, "FloorRepository::Delete"))
      throw std::runtime_error("[RunPrepared exception]");

    database->CommitTransaction();

    return true;
  } catch (const std::exception &e) {
    database->RollbackTransaction();
    throw(
        std::runtime_error(std::string("[Delete Exception]") + " " + e.what()));
  }
}
} // namespace omnisphere::repositories