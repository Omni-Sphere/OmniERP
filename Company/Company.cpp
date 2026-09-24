#include "Company/Company.hpp"
#include "Company/Repositories/Company.hpp"
#include <OmniData/DataTable.hpp>
#include <OmniData/Database.hpp>
#include <OmniData/DataMapper.hpp>

namespace omnisphere::services {
struct Company::Impl {
  explicit Impl(std::shared_ptr<omnisphere::data::Database> database)
      : repository(database) {}

  omnisphere::repositories::Company repository;
};

Company::Company(std::shared_ptr<omnisphere::data::Database> database)
    : pimpl(std::make_unique<Impl>(database)) {}

Company::~Company() = default;

omnisphere::models::Company
Company::Get(const omnisphere::dtos::GetCompany &_company, const std::vector<std::string>& fields) const {
  omnisphere::types::DataTable dataTable = pimpl->repository.Read(_company, fields);

  if (dataTable.IsEmpty()) {
    throw std::runtime_error("Business Configuration not found.");
  }

  return omnisphere::types::FromDataRow<omnisphere::models::Company>(dataTable[0]);
}

bool Company::Add(const omnisphere::dtos::CreateCompany &_company) const {
  return pimpl->repository.Create(_company);
}

bool Company::Modify(const omnisphere::dtos::UpdateCompany &_company) const {
  return pimpl->repository.Update(_company);
}

} // namespace omnisphere::services
