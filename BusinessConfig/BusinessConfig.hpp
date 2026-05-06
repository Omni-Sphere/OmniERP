#include <Database.hpp>
#include <DataTable.hpp>
#pragma once

#include <memory>
#include <Database.hpp>
#include <vector>

#include <Database.hpp>
#include <BusinessConfig/DTOs/CreateBusinessConfig.hpp>
#include <BusinessConfig/DTOs/GetBusinessConfig.hpp>
#include <BusinessConfig/DTOs/UpdateBusinessConfig.hpp>
#include <BusinessConfig/Models/BusinessConfig.hpp>

namespace omnisphere::services {

class BusinessConfig {
public:
  explicit BusinessConfig(std::shared_ptr<omnisphere::services::Database> database);
  ~BusinessConfig();

  omnisphere::models::BusinessConfig Get(const omnisphere::dtos::GetBusinessConfig &_businessConfig) const;
  bool Add(const omnisphere::dtos::CreateBusinessConfig &_businessConfig) const;
  bool Modify(const omnisphere::dtos::UpdateBusinessConfig &_businessConfig) const;

private:
  struct Impl;
  std::unique_ptr<Impl> pimpl;
};

} // namespace omnisphere::services