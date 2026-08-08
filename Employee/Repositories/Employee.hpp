#pragma once
#include "Employee/DTOs/Employee.hpp"
#include <OmniData/Database.hpp>
#include <OmniData/DataTable.hpp>

namespace omnisphere::repositories {
class EmployeeRepository {
public:
  explicit EmployeeRepository(
      std::shared_ptr<omnisphere::data::Database> Database);
  ~EmployeeRepository();

  bool Create(const omnisphere::dtos::CreateEmployee &employee) const;
  bool Update(const omnisphere::dtos::UpdateEmployee &employee) const;
  omnisphere::types::DataTable ReadAll() const;
  omnisphere::types::DataTable
  Read(const omnisphere::dtos::GetEmployee &getEmployee) const;
  bool Delete(int entry) const;

private:
  std::shared_ptr<omnisphere::data::Database> database;
  int GetCurrentSequence() const;
  bool UpdateSequence() const;
};
} // namespace omnisphere::repositories
