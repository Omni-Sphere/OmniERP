#include <Database.hpp>
#include <DataTable.hpp>
#pragma once 

#include <memory>
#include <Database.hpp>
#include <optional>
#include <string>

#include <Database.hpp>
#include <Company/DTOs/CreateCompany.hpp>
#include <Company/DTOs/UpdateCompany.hpp>
#include <Company/DTOs/GetCompany.hpp>
#include <DataTable.hpp>

namespace omnisphere::repositories {
class Company {
public:
  explicit Company(std::shared_ptr<omnisphere::services::Database> database);
  ~Company();

  bool Create(const omnisphere::dtos::CreateCompany &_company) const;
  bool Update(const omnisphere::dtos::UpdateCompany &_company) const;
  omnisphere::types::DataTable Read(const omnisphere::dtos::GetCompany &_company) const;

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
