#pragma once 

#include <memory>
#include <optional>
#include <string>

#include "Database.hpp"
#include "DTOs/CreateBusinessConfig.hpp"
#include "DTOs/UpdateBusinessConfig.hpp"
#include "DTOs/GetBusinessConfig.hpp"
#include "DataTable.hpp"

namespace omnisphere::repositories {
class BusinessConfig {
public:
  explicit BusinessConfig(std::shared_ptr<omnisphere::services::Database> database);
  ~BusinessConfig();

  bool Create(const omnisphere::dtos::CreateBusinessConfig &_businessConfig) const;
  bool Update(const omnisphere::dtos::UpdateBusinessConfig &_businessConfig) const;
  omnisphere::types::DataTable Read(const omnisphere::dtos::GetBusinessConfig &_businessConfig) const;

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
};
} // namespace omnisphere::repositories
