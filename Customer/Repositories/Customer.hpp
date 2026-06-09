#include <Database.hpp>
#include <DataTable.hpp>
#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <Customer/DTOs/CreateCustomer.hpp>
#include <Customer/DTOs/UpdateCustomer.hpp>

namespace omnisphere::repositories
{
    class Customer
    {
        public:
        explicit Customer(std::shared_ptr<omnisphere::services::Database> database);
        ~Customer();

        bool Create(const omnisphere::dtos::CreateCustomer &_customer) const;
        bool Update(const omnisphere::dtos::UpdateCustomer &_customer) const;
        omnisphere::types::DataTable Read(int entry) const;
        omnisphere::types::DataTable ReadAll() const;

        private:
        std::shared_ptr<omnisphere::services::Database> Database;

        template <typename T>
        void AddInsertParam(const std::string &field, const T &value,
                            std::vector<std::string> &insertClauses,
                            std::vector<omnisphere::types::SQLParam> &params) const;

        template <typename T>
        void AddSetParam(const std::string &field, const T &value,
                         std::vector<std::string> &setClauses,
                         std::vector<omnisphere::types::SQLParam> &params) const;

        int GetCurrentSequence() const;
        bool UpdateCustomerSequence() const;
    };
} // namespace omnisphere::repositories
