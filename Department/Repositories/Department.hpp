#pragma once
#include <OmniData/Database.hpp>
#include <OmniData/DataTable.hpp>
#include "Department/DTOs/Department.hpp"

namespace omnisphere::repositories {
class DepartmentRepository {
public:
  explicit DepartmentRepository(
      std::shared_ptr<omnisphere::data::Database> Database);
  ~DepartmentRepository();

  bool Create(const omnisphere::dtos::CreateDepartment &department) const;
  bool Update(const omnisphere::dtos::UpdateDepartment &department) const;
  omnisphere::types::DataTable ReadAll() const;
  omnisphere::types::DataTable
  Read(const omnisphere::dtos::GetDepartment &getDepartment) const;
  bool Delete(int entry) const;

private:
  std::shared_ptr<omnisphere::data::Database> database;
  int GetCurrentSequence() const;
  bool UpdateSequence() const;
};
} // namespace omnisphere::repositories
