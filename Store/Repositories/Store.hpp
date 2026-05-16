#include <Database.hpp>
#include <DataTable.hpp>
#pragma once 

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <Store/DTOs/CreateStore.hpp>
#include <Store/DTOs/UpdateStore.hpp>
#include <Store/DTOs/GetStore.hpp>

namespace omnisphere::repositories {
class Store {
public:
  explicit Store(std::shared_ptr<omnisphere::services::Database> database);
  ~Store();

  bool Create(const omnisphere::dtos::CreateStore &_store) const;
  bool Update(const omnisphere::dtos::UpdateStore &_store) const;
  omnisphere::types::DataTable Read(int entry) const;
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
