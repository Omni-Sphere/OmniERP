#include <Database.hpp>
#include <DataTable.hpp>
#pragma once
#include <UserModule/Models/UserModule.hpp>
#include <UserModule/DTOs/UserModule.hpp>

namespace omnisphere::usermodule {
class UserModule {
public:
    explicit UserModule(std::shared_ptr<omnisphere::services::Database> database);
    ~UserModule();

    omnisphere::models::UserModule Add(const omnisphere::dtos::CreateUserModule& userModule) const;
    omnisphere::models::UserModule Modify(const omnisphere::dtos::UpdateUserModule& userModule) const;
    std::vector<omnisphere::models::UserModule> GetAll() const;
    omnisphere::models::UserModule Get(const omnisphere::dtos::GetUserModule& getUserModule) const;
    bool Remove(int entry) const;

private:
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};
} // namespace omnisphere::usermodule
