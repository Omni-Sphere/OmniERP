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

namespace omnisphere::repositories
{
    class Store
    {
        public:
        explicit Store(std::shared_ptr<omnisphere::services::Database> database);
        ~Store();

        bool Create(const omnisphere::dtos::CreateStore &_store) const;
        bool Update(const omnisphere::dtos::UpdateStore &_store) const;
        omnisphere::types::DataTable Read(int entry, const std::vector<std::string>& fields = {}) const;
        omnisphere::types::DataTable ReadByCode(const std::string& code, const std::vector<std::string>& fields = {}) const;
        omnisphere::types::DataTable ReadAll(const std::vector<std::string>& fields = {}) const;
        omnisphere::types::DataTable Search(const std::vector<std::string>& fields, const omnisphere::dtos::GetStore &filter) const;

        private:
        std::shared_ptr<omnisphere::services::Database> Database;

        int GetCurrentSequence() const;
        bool UpdateStoreSequence() const;
    };
} // namespace omnisphere::repositories
