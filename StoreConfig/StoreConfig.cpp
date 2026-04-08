#include "StoreConfig.hpp"
#include "Repositories/StoreConfig.hpp"
#include "DataTable.hpp"

namespace omnisphere::services {

struct StoreConfig::Impl {
  explicit Impl(std::shared_ptr<omnisphere::services::Database> database)
      : repository(database) {}

  omnisphere::repositories::StoreConfig repository;
};

StoreConfig::StoreConfig(std::shared_ptr<omnisphere::services::Database> database)
    : pimpl(std::make_unique<Impl>(database)) {}

StoreConfig::~StoreConfig() = default;

omnisphere::models::StoreConfig StoreConfig::Get(const omnisphere::dtos::GetStoreConfig &_storeConfig) const {
  omnisphere::types::DataTable dataTable = pimpl->repository.Read(_storeConfig);
  if (dataTable.IsEmpty()) {
    throw std::runtime_error("Store configuration not found.");
  }

  return omnisphere::models::StoreConfig(
      dataTable[0]["Entry"], 
      dataTable[0]["Code"], 
      dataTable[0]["Name"],
      dataTable[0]["Address"].GetOptional<std::string>(),
      1, // Placeholder for CreatedBy
      "2024-01-01", // Placeholder for CreateDate
      std::nullopt,
      std::nullopt
  );
}

std::vector<omnisphere::models::StoreConfig> StoreConfig::Search(const omnisphere::dtos::SearchStoreConfig &_storeConfig) const {
  omnisphere::types::DataTable dataTable = pimpl->repository.Read(_storeConfig);
  std::vector<omnisphere::models::StoreConfig> results;

  for (int i = 0; i < dataTable.RowsCount(); i++) {
    results.emplace_back(
        dataTable[i]["Entry"], 
        dataTable[i]["Code"], 
        dataTable[i]["Name"],
        dataTable[i]["Address"].GetOptional<std::string>(),
        1, "2024-01-01", std::nullopt, std::nullopt
    );
  }
  return results;
}

std::vector<omnisphere::models::StoreConfig> StoreConfig::GetAll() const {
  omnisphere::types::DataTable dataTable = pimpl->repository.ReadAll();
  std::vector<omnisphere::models::StoreConfig> results;

  for (int i = 0; i < dataTable.RowsCount(); i++) {
    results.emplace_back(
        dataTable[i]["Entry"], 
        dataTable[i]["Code"], 
        dataTable[i]["Name"],
        dataTable[i]["Address"].GetOptional<std::string>(),
        1, "2024-01-01", std::nullopt, std::nullopt
    );
  }
  return results;
}

bool StoreConfig::Add(const omnisphere::dtos::CreateStoreConfig &_storeConfig) const {
  return pimpl->repository.Create(_storeConfig);
}

bool StoreConfig::Modify(const omnisphere::dtos::UpdateStoreConfig &_storeConfig) const {
  return pimpl->repository.Update(_storeConfig);
}

} // namespace omnisphere::services
