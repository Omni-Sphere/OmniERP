#include <Database.hpp>
#include <DataTable.hpp>
#pragma once
#include <DataTable.hpp>
#include <Department/DTOs/Department.hpp>

namespace omnisphere::repositories {
class DepartmentRepository {
public:
    explicit DepartmentRepository(std::shared_ptr<omnisphere::services::Database> Database);
    ~DepartmentRepository();

    bool Create(const omnisphere::dtos::CreateDepartment& department) const;
    bool Update(const omnisphere::dtos::UpdateDepartment& department) const;
    omnisphere::types::DataTable ReadAll() const;
    omnisphere::types::DataTable Read(const omnisphere::dtos::GetDepartment& getDepartment) const;
    bool Delete(int entry) const;

private:
    std::shared_ptr<omnisphere::services::Database> database;
    int GetCurrentSequence() const;
    bool UpdateSequence() const;
};
} // namespace omnisphere::repositories
