#include <Database.hpp>
#include <DataTable.hpp>
#pragma once
#include <DataTable.hpp>
#include <Role/DTOs/Role.hpp>

namespace omnisphere::repositories
{
    class RoleRepository
    {
        public:
        explicit RoleRepository(std::shared_ptr<omnisphere::services::Database> Database);
        ~RoleRepository();

        bool Create(const omnisphere::dtos::CreateRole& role) const;
        bool Update(const omnisphere::dtos::UpdateRole& role) const;
        omnisphere::types::DataTable ReadAll() const;
        omnisphere::types::DataTable Read(const omnisphere::dtos::GetRole& getRole) const;
        bool Delete(int entry) const;

        private:
        std::shared_ptr<omnisphere::services::Database> database;
        int GetCurrentSequence() const;
        bool UpdateSequence() const;
    };
} // namespace omnisphere::repositories
