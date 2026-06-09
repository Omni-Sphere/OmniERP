#include <Database.hpp>
#include <DataTable.hpp>
#pragma once

#include <memory>
#include <Database.hpp>
#include <vector>

#include <Database.hpp>
#include <Company/DTOs/CreateCompany.hpp>
#include <Company/DTOs/GetCompany.hpp>
#include <Company/DTOs/UpdateCompany.hpp>
#include <Company/Models/Company.hpp>

namespace omnisphere::services
{
    class Company
    {
        public:
        explicit Company(std::shared_ptr<omnisphere::services::Database> database);
        ~Company();

        omnisphere::models::Company Get(const omnisphere::dtos::GetCompany &_company) const;
        bool Add(const omnisphere::dtos::CreateCompany &_company) const;
        bool Modify(const omnisphere::dtos::UpdateCompany &_company) const;

        private:
        struct Impl;
        std::unique_ptr<Impl> pimpl;
    };

} // namespace omnisphere::services