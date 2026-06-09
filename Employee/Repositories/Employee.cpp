#include <Database.hpp>
#include <DataTable.hpp>
#include <DataTable.hpp>
#include <Database.hpp>
#include <DataTable.hpp>
#include <Employee/Repositories/Employee.hpp>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <SQLParams.hpp>

namespace omnisphere::repositories
{
    EmployeeRepository::EmployeeRepository(std::shared_ptr<omnisphere::services::Database> Database)
    : database(Database) {}

    EmployeeRepository::~EmployeeRepository() = default;

    bool EmployeeRepository::Create(const omnisphere::dtos::CreateEmployee& employee) const
    {
        try
        {
            const std::string query = "INSERT INTO Employees (Entry, Code, Name, FirstName, SecondName, LastName, SecondLastName, Phone, DateOfBirth, PlaceOfBirth, Comments, CreatedBy, CreateDate, IsActive) "
            "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, GETDATE(), 'Y')";

            std::vector<omnisphere::types::SQLParam> parameters = {
                omnisphere::types::MakeSQLParam(GetCurrentSequence()),
                omnisphere::types::MakeSQLParam(employee.Code),
                omnisphere::types::MakeSQLParam(employee.Name),
                omnisphere::types::MakeSQLParam(employee.FirstName),
                omnisphere::types::MakeSQLParam(employee.SecondName),
                omnisphere::types::MakeSQLParam(employee.LastName),
                omnisphere::types::MakeSQLParam(employee.SecondLastName),
                omnisphere::types::MakeSQLParam(employee.Phone),
                omnisphere::types::MakeSQLParam(employee.DateOfBirth),
                omnisphere::types::MakeSQLParam(employee.PlaceOfBirth),
                omnisphere::types::MakeSQLParam(employee.Comments),
                omnisphere::types::MakeSQLParam(employee.CreatedBy)};

            if (!database->RunPrepared(query, parameters, "EmployeeRepository::Create"))
                throw std::runtime_error("[RunPrepared exception]");

            if (!UpdateSequence())
                throw std::runtime_error("[UpdateSequence exception]");

            database->CommitTransaction();

            return true;
        }
        catch (const std::exception& e)
        {
            database->RollbackTransaction();
            throw(std::runtime_error(std::string("[CreateEmployee Exception] ") + e.what()));
        }
    }

    bool EmployeeRepository::Update(const omnisphere::dtos::UpdateEmployee& employee) const
    {
        try
        {
            std::string query = "UPDATE Employees SET UpdateDate = GETDATE()";
            std::vector<omnisphere::types::SQLParam> parameters;

            if (employee.Code.has_value())
            { query += ", Code = ?"; parameters.push_back(omnisphere::types::MakeSQLParam(employee.Code.value())); }

            if (employee.Name.has_value())
            { query += ", Name = ?"; parameters.push_back(omnisphere::types::MakeSQLParam(employee.Name.value())); }

            if (employee.FirstName.has_value())
            { query += ", FirstName = ?"; parameters.push_back(omnisphere::types::MakeSQLParam(employee.FirstName.value())); }

            if (employee.SecondName.has_value())
            { query += ", SecondName = ?"; parameters.push_back(omnisphere::types::MakeSQLParam(employee.SecondName.value())); }

            if (employee.LastName.has_value())
            { query += ", LastName = ?"; parameters.push_back(omnisphere::types::MakeSQLParam(employee.LastName.value())); }

            if (employee.SecondLastName.has_value())
            { query += ", SecondLastName = ?"; parameters.push_back(omnisphere::types::MakeSQLParam(employee.SecondLastName.value())); }

            if (employee.Phone.has_value())
            { query += ", Phone = ?"; parameters.push_back(omnisphere::types::MakeSQLParam(employee.Phone.value())); }

            if (employee.DateOfBirth.has_value())
            { query += ", DateOfBirth = ?"; parameters.push_back(omnisphere::types::MakeSQLParam(employee.DateOfBirth.value())); }

            if (employee.PlaceOfBirth.has_value())
            { query += ", PlaceOfBirth = ?"; parameters.push_back(omnisphere::types::MakeSQLParam(employee.PlaceOfBirth.value())); }

            if (employee.Comments.has_value())
            { query += ", Comments = ?"; parameters.push_back(omnisphere::types::MakeSQLParam(employee.Comments.value())); }

            if (employee.IsActive.has_value())
            { query += ", IsActive = ?"; parameters.push_back(omnisphere::types::MakeSQLParam(employee.IsActive.value() ? "Y" : "N")); }

            query += ", LastUpdatedBy = ? WHERE Entry = ?";
            parameters.push_back(omnisphere::types::MakeSQLParam(employee.UpdatedBy));
            parameters.push_back(omnisphere::types::MakeSQLParam(employee.Entry));

            if (!database->RunPrepared(query, parameters, "EmployeeRepository::Update"))
                throw std::runtime_error("[RunPrepared exception]");

            database->CommitTransaction();

            return true;
        }
        catch (const std::exception& e)
        {
            database->RollbackTransaction();
            throw(std::runtime_error(std::string("[UpdateEmployee Exception] ") + e.what()));
        }
    }

    omnisphere::types::DataTable EmployeeRepository::ReadAll() const
    {
        try
        {
            const std::string query = "SELECT * FROM Employees WHERE IsActive = 'Y'";

            return database->FetchResults(query, "EmployeeRepository::ReadAll");
        }
        catch (const std::exception& e)
        {
            throw(std::runtime_error(std::string("[ReadAllEmployee Exception] ") + e.what()));
        }
    }

    omnisphere::types::DataTable EmployeeRepository::Read(const omnisphere::dtos::GetEmployee& getEmployee) const
    {
        try
        {
            std::string query = "SELECT * FROM Employees WHERE 1=1";
            std::vector<omnisphere::types::SQLParam> parameters;

            if (getEmployee.Entry.has_value())
            {
                query += " AND Entry = ?";
                parameters.push_back(omnisphere::types::MakeSQLParam(getEmployee.Entry.value()));
            }

            if (getEmployee.Code.has_value())
            {
                query += " AND Code = ?";
                parameters.push_back(omnisphere::types::MakeSQLParam(getEmployee.Code.value()));
            }

            return database->FetchPrepared(query, parameters, "EmployeeRepository::Read");
        }
        catch (const std::exception& e)
        {
            throw(std::runtime_error(std::string("[ReadEmployee Exception] ") + e.what()));
        }
    }

    bool EmployeeRepository::Delete(int entry) const
    {
        try
        {
            const std::string query = "UPDATE Employees SET IsActive = 'N' WHERE Entry = ?";
            std::vector<omnisphere::types::SQLParam> parameters = {omnisphere::types::MakeSQLParam(entry)};

            if (!database->RunPrepared(query, parameters, "EmployeeRepository::Delete"))
                throw std::runtime_error("[RunPrepared exception]");

            database->CommitTransaction();

            return true;
        }
        catch (const std::exception& e)
        {
            database->RollbackTransaction();
            throw(std::runtime_error(std::string("[DeleteEmployee Exception] ") + e.what()));
        }
    }

    int EmployeeRepository::GetCurrentSequence() const
    {
        try
        {
            const std::string query = "SELECT ISNULL(EmpSequence, 0) + 1 EmpSequence FROM Sequences WHERE SeqEntry = 1";
            omnisphere::types::DataTable dataTable = database->FetchResults(query, "EmployeeRepository::GetCurrentSequence");

            return dataTable[0]["EmpSequence"];
        }
        catch (const std::exception& e)
        {
            throw(std::runtime_error(std::string("[GetCurrentSequence Exception] ") + e.what()));
        }
    }

    bool EmployeeRepository::UpdateSequence() const
    {
        try
        {
            const std::string query = "UPDATE Sequences SET EmpSequence = ISNULL(EmpSequence, 0) + 1 WHERE SeqEntry = 1";

            if (!database->RunStatement(query, "EmployeeRepository::UpdateSequence"))
                throw std::runtime_error("[RunStatement exception]");

            return true;
        }
        catch (const std::exception& e)
        {
            throw(std::runtime_error(std::string("[UpdateSequence Exception] ") + e.what()));
        }
    }
} // namespace omnisphere::repositories
