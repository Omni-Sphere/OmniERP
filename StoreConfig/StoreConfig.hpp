#include <Database.hpp>
#include <DataTable.hpp>
#pragma once

#include <memory>
#include <Database.hpp>
#include <vector>

#include <Database.hpp>
#include <StoreConfig/DTOs/CreateStoreConfig.hpp>
#include <StoreConfig/DTOs/GetStoreConfig.hpp>
#include <StoreConfig/DTOs/SearchStoreConfig.hpp>
#include <StoreConfig/DTOs/UpdateStoreConfig.hpp>
#include <StoreConfig/Models/StoreConfig.hpp>

namespace omnisphere::services {

class StoreConfig {
public:
  explicit StoreConfig(std::shared_ptr<omnisphere::services::Database> database);
  ~StoreConfig();

  omnisphere::models::StoreConfig Get(const omnisphere::dtos::GetStoreConfig &_storeConfig) const;
  std::vector<omnisphere::models::StoreConfig> Search(const omnisphere::dtos::SearchStoreConfig &_storeConfig) const;
  std::vector<omnisphere::models::StoreConfig> GetAll() const;
  bool Add(const omnisphere::dtos::CreateStoreConfig &_storeConfig) const;
  bool Modify(const omnisphere::dtos::UpdateStoreConfig &_storeConfig) const;

private:
  struct Impl;
  std::unique_ptr<Impl> pimpl;
};

} // namespace omnisphere::services
