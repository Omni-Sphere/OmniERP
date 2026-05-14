#include <Database.hpp>
#include <DataTable.hpp>
#include <DataTable.hpp>
#include <Database.hpp>
#include <DataTable.hpp>
#include <RoleModule/Repositories/RoleModule.hpp>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <SQLParams.hpp>

namespace omnisphere::repositories {
RoleModuleRepository::RoleModuleRepository(std::shared_ptr<omnisphere::services::Database> Database) : database(Database) {}
RoleModuleRepository::~RoleModuleRepository() = default;

bool RoleModuleRepository::Create(const omnisphere::dtos::CreateRoleModule& roleModule) const {
    try {
        const std::string query = "INSERT INTO RoleModules (Entry, RoleEntry, ModuleEntry, IsAllowed, CreatedBy, CreateDate) VALUES (?, ?, ?, ?, ?, GETDATE())";
        std::vector<omnisphere::types::SQLParam> parameters = {
            omnisphere::types::MakeSQLParam(GetCurrentSequence()),
            omnisphere::types::MakeSQLParam(roleModule.RoleEntry),
            omnisphere::types::MakeSQLParam(roleModule.ModuleEntry),
            omnisphere::types::MakeSQLParam(roleModule.IsAllowed ? "Y" : "N"),
            omnisphere::types::MakeSQLParam(roleModule.CreatedBy)};
        if (!database->RunPrepared(query, parameters, "RoleModuleRepository::Create")) throw std::runtime_error("[RunPrepared exception]");
        if (!UpdateSequence()) throw std::runtime_error("[UpdateSequence exception]");
        database->CommitTransaction();
        return true;
    } catch (const std::exception& e) { database->RollbackTransaction(); throw(std::runtime_error(std::string("[CreateRoleModule Exception] ") + e.what())); }
}

bool RoleModuleRepository::Update(const omnisphere::dtos::UpdateRoleModule& roleModule) const {
    try {
        std::string query = "UPDATE RoleModules SET UpdateDate = GETDATE()";
        std::vector<omnisphere::types::SQLParam> parameters;
        if (roleModule.IsAllowed.has_value()) { query += ", IsAllowed = ?"; parameters.push_back(omnisphere::types::MakeSQLParam(roleModule.IsAllowed.value() ? "Y" : "N")); }
        query += ", LastUpdatedBy = ? WHERE Entry = ?";
        parameters.push_back(omnisphere::types::MakeSQLParam(roleModule.UpdatedBy));
        parameters.push_back(omnisphere::types::MakeSQLParam(roleModule.Entry));
        if (!database->RunPrepared(query, parameters, "RoleModuleRepository::Update")) throw std::runtime_error("[RunPrepared exception]");
        database->CommitTransaction();
        return true;
    } catch (const std::exception& e) { database->RollbackTransaction(); throw(std::runtime_error(std::string("[UpdateRoleModule Exception] ") + e.what())); }
}

omnisphere::types::DataTable RoleModuleRepository::ReadAll() const {
    try { return database->FetchResults("SELECT * FROM RoleModules", "RoleModuleRepository::ReadAll"); }
    catch (const std::exception& e) { throw(std::runtime_error(std::string("[ReadAllRoleModule Exception] ") + e.what())); }
}

omnisphere::types::DataTable RoleModuleRepository::Read(const omnisphere::dtos::GetRoleModule& getRoleModule) const {
    try {
        std::string query = "SELECT * FROM RoleModules WHERE 1=1";
        std::vector<omnisphere::types::SQLParam> parameters;
        if (getRoleModule.Entry.has_value()) { query += " AND Entry = ?"; parameters.push_back(omnisphere::types::MakeSQLParam(getRoleModule.Entry.value())); }
        if (getRoleModule.RoleEntry.has_value()) { query += " AND RoleEntry = ?"; parameters.push_back(omnisphere::types::MakeSQLParam(getRoleModule.RoleEntry.value())); }
        if (getRoleModule.ModuleEntry.has_value()) { query += " AND ModuleEntry = ?"; parameters.push_back(omnisphere::types::MakeSQLParam(getRoleModule.ModuleEntry.value())); }
        return database->FetchPrepared(query, parameters, "RoleModuleRepository::Read");
    } catch (const std::exception& e) { throw(std::runtime_error(std::string("[ReadRoleModule Exception] ") + e.what())); }
}

bool RoleModuleRepository::Delete(int entry) const {
    try {
        std::vector<omnisphere::types::SQLParam> parameters = {omnisphere::types::MakeSQLParam(entry)};
        if (!database->RunPrepared("DELETE FROM RoleModules WHERE Entry = ?", parameters, "RoleModuleRepository::Delete")) throw std::runtime_error("[RunPrepared exception]");
        database->CommitTransaction();
        return true;
    } catch (const std::exception& e) { database->RollbackTransaction(); throw(std::runtime_error(std::string("[DeleteRoleModule Exception] ") + e.what())); }
}

int RoleModuleRepository::GetCurrentSequence() const {
    try {
        omnisphere::types::DataTable dataTable = database->FetchResults("SELECT ISNULL(RoleModSequence, 0) + 1 RoleModSequence FROM Sequences WHERE SeqEntry = 1");
        return dataTable[0]["RoleModSequence"];
    } catch (const std::exception& e) { throw(std::runtime_error(std::string("[GetCurrentSequence Exception] ") + e.what())); }
}

bool RoleModuleRepository::UpdateSequence() const {
    try {
        if (!database->RunStatement("UPDATE Sequences SET RoleModSequence = ISNULL(RoleModSequence, 0) + 1 WHERE SeqEntry = 1")) throw std::runtime_error("[RunStatement exception]");
        return true;
    } catch (const std::exception& e) { throw(std::runtime_error(std::string("[UpdateSequence Exception] ") + e.what())); }
}
}
