#pragma once

#include <memory>
#include <vector>

#include "Database.hpp"
#include "DTOs/CreateStoreConfig.hpp"
#include "DTOs/GetStoreConfig.hpp"
#include "DTOs/SearchStoreConfig.hpp"
#include "DTOs/UpdateStoreConfig.hpp"
#include "Models/StoreConfig.hpp"

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
