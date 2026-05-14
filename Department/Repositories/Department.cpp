#include <Database.hpp>
#include <DataTable.hpp>
#include <DataTable.hpp>
#include <Database.hpp>
#include <DataTable.hpp>
#include <Department/Repositories/Department.hpp>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <SQLParams.hpp>

namespace omnisphere::repositories {
DepartmentRepository::DepartmentRepository(std::shared_ptr<omnisphere::services::Database> Database)
    : database(Database) {}

DepartmentRepository::~DepartmentRepository() = default;

bool DepartmentRepository::Create(const omnisphere::dtos::CreateDepartment& department) const {
    try {
        const std::string query = "INSERT INTO Departments (Entry, Code, Name, CreatedBy, CreateDate, IsActive) VALUES (?, ?, ?, ?, GETDATE(), 'Y')";

        std::vector<omnisphere::types::SQLParam> parameters = {
            omnisphere::types::MakeSQLParam(GetCurrentSequence()),
            omnisphere::types::MakeSQLParam(department.Code),
            omnisphere::types::MakeSQLParam(department.Name),
            omnisphere::types::MakeSQLParam(department.CreatedBy)};

        if (!database->RunPrepared(query, parameters, "DepartmentRepository::Create"))
            throw std::runtime_error("[RunPrepared exception]");

        if (!UpdateSequence())
            throw std::runtime_error("[UpdateSequence exception]");

        database->CommitTransaction();
        return true;
    } catch (const std::exception& e) {
        database->RollbackTransaction();
        throw(std::runtime_error(std::string("[CreateDepartment Exception] ") + e.what()));
    }
}

bool DepartmentRepository::Update(const omnisphere::dtos::UpdateDepartment& department) const {
    try {
        std::string query = "UPDATE Departments SET UpdateDate = GETDATE()";
        std::vector<omnisphere::types::SQLParam> parameters;

        if (department.Code.has_value()) {
            query += ", Code = ?";
            parameters.push_back(omnisphere::types::MakeSQLParam(department.Code.value()));
        }
        if (department.Name.has_value()) {
            query += ", Name = ?";
            parameters.push_back(omnisphere::types::MakeSQLParam(department.Name.value()));
        }
        if (department.IsActive.has_value()) {
            query += ", IsActive = ?";
            parameters.push_back(omnisphere::types::MakeSQLParam(department.IsActive.value() ? "Y" : "N"));
        }

        query += ", LastUpdatedBy = ? WHERE Entry = ?";
        parameters.push_back(omnisphere::types::MakeSQLParam(department.UpdatedBy));
        parameters.push_back(omnisphere::types::MakeSQLParam(department.Entry));

        if (!database->RunPrepared(query, parameters, "DepartmentRepository::Update"))
            throw std::runtime_error("[RunPrepared exception]");

        database->CommitTransaction();
        return true;
    } catch (const std::exception& e) {
        database->RollbackTransaction();
        throw(std::runtime_error(std::string("[UpdateDepartment Exception] ") + e.what()));
    }
}

omnisphere::types::DataTable DepartmentRepository::ReadAll() const {
    try {
        const std::string query = "SELECT Entry, Code, Name, IsActive, CreatedBy, CreateDate, LastUpdatedBy, UpdateDate FROM Departments WHERE IsActive = 'Y'";
        return database->FetchResults(query, "DepartmentRepository::ReadAll");
    } catch (const std::exception& e) {
        throw(std::runtime_error(std::string("[ReadAllDepartment Exception] ") + e.what()));
    }
}

omnisphere::types::DataTable DepartmentRepository::Read(const omnisphere::dtos::GetDepartment& getDepartment) const {
    try {
        std::string query = "SELECT Entry, Code, Name, IsActive, CreatedBy, CreateDate, LastUpdatedBy, UpdateDate FROM Departments WHERE 1=1";
        std::vector<omnisphere::types::SQLParam> parameters;

        if (getDepartment.Entry.has_value()) {
            query += " AND Entry = ?";
            parameters.push_back(omnisphere::types::MakeSQLParam(getDepartment.Entry.value()));
        }
        if (getDepartment.Code.has_value()) {
            query += " AND Code = ?";
            parameters.push_back(omnisphere::types::MakeSQLParam(getDepartment.Code.value()));
        }
        if (getDepartment.Name.has_value()) {
            query += " AND Name = ?";
            parameters.push_back(omnisphere::types::MakeSQLParam(getDepartment.Name.value()));
        }

        return database->FetchPrepared(query, parameters, "DepartmentRepository::Read");
    } catch (const std::exception& e) {
        throw(std::runtime_error(std::string("[ReadDepartment Exception] ") + e.what()));
    }
}

bool DepartmentRepository::Delete(int entry) const {
    try {
        const std::string query = "UPDATE Departments SET IsActive = 'N' WHERE Entry = ?";
        std::vector<omnisphere::types::SQLParam> parameters = {omnisphere::types::MakeSQLParam(entry)};

        if (!database->RunPrepared(query, parameters, "DepartmentRepository::Delete"))
            throw std::runtime_error("[RunPrepared exception]");

        database->CommitTransaction();
        return true;
    } catch (const std::exception& e) {
        database->RollbackTransaction();
        throw(std::runtime_error(std::string("[DeleteDepartment Exception] ") + e.what()));
    }
}

int DepartmentRepository::GetCurrentSequence() const {
    try {
        const std::string query = "SELECT ISNULL(DeptSequence, 0) + 1 DeptSequence FROM Sequences WHERE SeqEntry = 1";
        omnisphere::types::DataTable dataTable = database->FetchResults(query, "DepartmentRepository::GetCurrentSequence");
        return dataTable[0]["DeptSequence"];
    } catch (const std::exception& e) {
        throw(std::runtime_error(std::string("[GetCurrentSequence Exception] ") + e.what()));
    }
}

bool DepartmentRepository::UpdateSequence() const {
    try {
        const std::string query = "UPDATE Sequences SET DeptSequence = ISNULL(DeptSequence, 0) + 1 WHERE SeqEntry = 1";
        if (!database->RunStatement(query, "DepartmentRepository::UpdateSequence"))
            throw std::runtime_error("[RunStatement exception]");
        return true;
    } catch (const std::exception& e) {
        throw(std::runtime_error(std::string("[UpdateSequence Exception] ") + e.what()));
    }
}
} // namespace omnisphere::repositories
