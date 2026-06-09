#include <Database.hpp>
#include <DataTable.hpp>
#pragma once
#include <DataTable.hpp>
#include <Permission/DTOs/Permission.hpp>

namespace omnisphere::repositories
{
    class PermissionRepository
    {
        public:
        explicit PermissionRepository(std::shared_ptr<omnisphere::services::Database> Database);
        ~PermissionRepository();

        omnisphere::types::DataTable ReadAll() const;
        omnisphere::types::DataTable Read(const omnisphere::dtos::GetPermission& getPermission) const;

        private:
        std::shared_ptr<omnisphere::services::Database> database;
    };
} // namespace omnisphere::repositories
