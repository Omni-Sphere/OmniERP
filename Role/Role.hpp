#include <Database.hpp>
#include <DataTable.hpp>
#pragma once
#include <Role/Models/Role.hpp>
#include <Role/DTOs/Role.hpp>

namespace omnisphere::role {
class Role {
public:
    explicit Role(std::shared_ptr<omnisphere::services::Database> database);
    ~Role();

    omnisphere::models::Role Add(const omnisphere::dtos::CreateRole& role) const;
    omnisphere::models::Role Modify(const omnisphere::dtos::UpdateRole& role) const;
    std::vector<omnisphere::models::Role> GetAll() const;
    omnisphere::models::Role Get(const omnisphere::dtos::GetRole& getRole) const;
    bool Remove(int entry) const;

private:
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};
} // namespace omnisphere::role
