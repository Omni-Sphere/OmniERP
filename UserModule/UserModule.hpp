#pragma once
#include <OmniData/Database.hpp>
#include "UserModule/DTOs/UserModule.hpp"
#include "UserModule/Models/UserModule.hpp"

namespace omnisphere::usermodule {
class UserModule {
public:
  explicit UserModule(std::shared_ptr<omnisphere::data::Database> database);
  ~UserModule();

  omnisphere::models::UserModule
  Add(const omnisphere::dtos::CreateUserModule &userModule) const;
  omnisphere::models::UserModule
  Modify(const omnisphere::dtos::UpdateUserModule &userModule) const;
  std::vector<omnisphere::models::UserModule> GetAll() const;
  omnisphere::models::UserModule
  Get(const omnisphere::dtos::GetUserModule &getUserModule) const;
  bool Remove(int entry) const;

private:
  struct Impl;
  std::unique_ptr<Impl> pImpl;
};
} // namespace omnisphere::usermodule
