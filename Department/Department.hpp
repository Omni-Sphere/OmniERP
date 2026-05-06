#include <Database.hpp>
#include <DataTable.hpp>
#pragma once
#include <Department/Models/Department.hpp>
#include <Department/DTOs/Department.hpp>

namespace omnisphere::department {
class Department {
public:
    explicit Department(std::shared_ptr<omnisphere::services::Database> database);
    ~Department();

    omnisphere::models::Department Add(const omnisphere::dtos::CreateDepartment& department) const;
    omnisphere::models::Department Modify(const omnisphere::dtos::UpdateDepartment& department) const;
    std::vector<omnisphere::models::Department> GetAll() const;
    omnisphere::models::Department Get(const omnisphere::dtos::GetDepartment& getDepartment) const;
    bool Remove(int entry) const;

private:
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};
} // namespace omnisphere::department
