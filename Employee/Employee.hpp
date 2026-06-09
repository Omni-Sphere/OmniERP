#include <Database.hpp>
#include <DataTable.hpp>
#pragma once
#include <Employee/Models/Employee.hpp>
#include <Employee/DTOs/Employee.hpp>

namespace omnisphere::employee
{
    class Employee
    {
        public:
        explicit Employee(std::shared_ptr<omnisphere::services::Database> database);
        ~Employee();

        omnisphere::models::Employee Add(const omnisphere::dtos::CreateEmployee& employee) const;
        omnisphere::models::Employee Modify(const omnisphere::dtos::UpdateEmployee& employee) const;
        std::vector<omnisphere::models::Employee> GetAll() const;
        omnisphere::models::Employee Get(const omnisphere::dtos::GetEmployee& getEmployee) const;
        bool Remove(int entry) const;

        private:
        struct Impl;
        std::unique_ptr<Impl> pImpl;
    };
} // namespace omnisphere::employee
