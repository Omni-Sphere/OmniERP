#include <Database.hpp>
#include <DataTable.hpp>
#pragma once
#include <DataTable.hpp>
#include <Employee/DTOs/Employee.hpp>

namespace omnisphere::repositories {
class EmployeeRepository {
public:
    explicit EmployeeRepository(std::shared_ptr<omnisphere::services::Database> Database);
    ~EmployeeRepository();

    bool Create(const omnisphere::dtos::CreateEmployee& employee) const;
    bool Update(const omnisphere::dtos::UpdateEmployee& employee) const;
    omnisphere::types::DataTable ReadAll() const;
    omnisphere::types::DataTable Read(const omnisphere::dtos::GetEmployee& getEmployee) const;
    bool Delete(int entry) const;

private:
    std::shared_ptr<omnisphere::services::Database> database;
    int GetCurrentSequence() const;
    bool UpdateSequence() const;
};
} // namespace omnisphere::repositories
