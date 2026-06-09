#include <Database.hpp>
#include <DataTable.hpp>
#pragma once

#include <memory>
#include <vector>

#include <Customer/DTOs/CreateCustomer.hpp>
#include <Customer/DTOs/UpdateCustomer.hpp>
#include <Customer/Models/Customer.hpp>

namespace omnisphere::services {

class Customer {
public:
  explicit Customer(std::shared_ptr<omnisphere::services::Database> database);
  ~Customer();

  std::optional<omnisphere::models::Customer> Get(int entry) const;
  std::vector<omnisphere::models::Customer> GetAll() const;
  bool Add(const omnisphere::dtos::CreateCustomer &_customer) const;
  bool Modify(const omnisphere::dtos::UpdateCustomer &_customer) const;

private:
  struct Impl;
  std::unique_ptr<Impl> pimpl;
};

} // namespace omnisphere::services
