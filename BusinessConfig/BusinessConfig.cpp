#include "BusinessConfig.hpp"
#include "Repositories/BusinessConfig.hpp"
#include "DataTable.hpp"

namespace omnisphere::services {

struct BusinessConfig::Impl {
  explicit Impl(std::shared_ptr<omnisphere::services::Database> database)
      : repository(database) {}

  omnisphere::repositories::BusinessConfig repository;
};

BusinessConfig::BusinessConfig(std::shared_ptr<omnisphere::services::Database> database)
    : pimpl(std::make_unique<Impl>(database)) {}

BusinessConfig::~BusinessConfig() = default;

omnisphere::models::BusinessConfig BusinessConfig::Get(const omnisphere::dtos::GetBusinessConfig &_businessConfig) const {
  omnisphere::types::DataTable dataTable = pimpl->repository.Read(_businessConfig);
  if (dataTable.IsEmpty()) {
    throw std::runtime_error("Business Configuration not found.");
  }

  return omnisphere::models::BusinessConfig(
      dataTable[0]["Entry"], 
      dataTable[0]["Code"], 
      dataTable[0]["Name"],
      dataTable[0]["Address"].GetOptional<std::string>(),
      dataTable[0]["Address2"].GetOptional<std::string>(),
      dataTable[0]["City"].GetOptional<int>(),
      dataTable[0]["State"].GetOptional<int>(),
      dataTable[0]["ZipCode"].GetOptional<int>(),
      dataTable[0]["Country"].GetOptional<int>(),
      dataTable[0]["TaxID"].GetOptional<std::string>(),
      dataTable[0]["Currency"],
      dataTable[0]["Phone1"].GetOptional<std::string>(),
      dataTable[0]["Phone2"].GetOptional<std::string>(),
      dataTable[0]["Email"].GetOptional<std::string>(),
      dataTable[0]["WebSite"].GetOptional<std::string>(),
      dataTable[0]["FacebookProfile"].GetOptional<std::string>(),
      dataTable[0]["InstagramProfile"].GetOptional<std::string>(),
      dataTable[0]["XProfile"].GetOptional<std::string>(),
      dataTable[0]["LogoFile"].GetOptional<std::string>(),
      dataTable[0]["AttachmentsPath"].GetOptional<std::string>(),
      1, // Placeholder for CreatedBy
      "2024-01-01", // Placeholder for CreateDate
      std::nullopt,
      std::nullopt
  );
}

bool BusinessConfig::Add(const omnisphere::dtos::CreateBusinessConfig &_businessConfig) const {
  return pimpl->repository.Create(_businessConfig);
}

bool BusinessConfig::Modify(const omnisphere::dtos::UpdateBusinessConfig &_businessConfig) const {
  return pimpl->repository.Update(_businessConfig);
}

} // namespace omnisphere::services
