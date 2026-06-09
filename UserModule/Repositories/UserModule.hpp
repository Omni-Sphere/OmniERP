#include <Database.hpp>
#include <DataTable.hpp>
#pragma once
#include <DataTable.hpp>
#include <UserModule/DTOs/UserModule.hpp>

namespace omnisphere::repositories
{
    class UserModuleRepository
    {
        public:
        explicit UserModuleRepository(std::shared_ptr<omnisphere::services::Database> Database);
        ~UserModuleRepository();

        bool Create(const omnisphere::dtos::CreateUserModule& userModule) const;
        bool Update(const omnisphere::dtos::UpdateUserModule& userModule) const;
        omnisphere::types::DataTable ReadAll() const;
        omnisphere::types::DataTable Read(const omnisphere::dtos::GetUserModule& getUserModule) const;
        bool Delete(int entry) const;

        private:
        std::shared_ptr<omnisphere::services::Database> database;
        int GetCurrentSequence() const;
        bool UpdateSequence() const;
    };
} // namespace omnisphere::repositories
