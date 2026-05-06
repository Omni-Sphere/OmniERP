#include <Database.hpp>
#include <DataTable.hpp>
#pragma once 

#include <memory>
#include <Database.hpp>
#include <optional>
#include <string>

#include <Database.hpp>
#include <StoreConfig/DTOs/CreateStoreConfig.hpp>
#include <StoreConfig/DTOs/UpdateStoreConfig.hpp>
#include <StoreConfig/DTOs/GetStoreConfig.hpp>
#include <StoreConfig/DTOs/SearchStoreConfig.hpp>
#include <DataTable.hpp>

namespace omnisphere::repositories {
class StoreConfig {
public:
  explicit StoreConfig(std::shared_ptr<omnisphere::services::Database> database);
  ~StoreConfig();

  bool Create(const omnisphere::dtos::CreateStoreConfig &_storeConfig) const;
  bool Update(const omnisphere::dtos::UpdateStoreConfig &_storeConfig) const;
  omnisphere::types::DataTable Read(const omnisphere::dtos::GetStoreConfig &_storeConfig) const;
  omnisphere::types::DataTable Read(const omnisphere::dtos::SearchStoreConfig &_storeConfig) const;
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
  bool UpdateStoreSequence() const;
};
} // namespace omnisphere::repositories
