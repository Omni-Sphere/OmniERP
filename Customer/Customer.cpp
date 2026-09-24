#include "Customer/Customer.hpp"
#include "Customer/Repositories/Customer.hpp"
#include <OmniData/DataTable.hpp>
#include <OmniData/Database.hpp>
#include <OmniData/DataMapper.hpp>

namespace omnisphere::services {
struct Customer::Impl {
  explicit Impl(std::shared_ptr<omnisphere::data::Database> database)
      : repository(database) {}

  omnisphere::repositories::Customer repository;
};

Customer::Customer(std::shared_ptr<omnisphere::data::Database> database)
    : pimpl(std::make_unique<Impl>(database)) {}

Customer::~Customer() = default;

std::optional<omnisphere::models::Customer> Customer::Get(int entry, const std::vector<std::string>& fields) const {
  omnisphere::types::DataTable dataTable = pimpl->repository.Read(entry, fields);

  if (dataTable.IsEmpty()) {
    return std::nullopt;
  }

  return omnisphere::types::FromDataRow<omnisphere::models::Customer>(dataTable[0]);
}

std::vector<omnisphere::models::Customer> Customer::GetAll(const std::vector<std::string>& fields) const {
  omnisphere::types::DataTable dataTable = pimpl->repository.ReadAll(fields);
  return omnisphere::types::DataTableToModels<omnisphere::models::Customer>(dataTable);
}

bool Customer::Add(const omnisphere::dtos::CreateCustomer &_customer) const {
  return pimpl->repository.Create(_customer);
}

bool Customer::Modify(const omnisphere::dtos::UpdateCustomer &_customer) const {
  return pimpl->repository.Update(_customer);
}

} // namespace omnisphere::services
