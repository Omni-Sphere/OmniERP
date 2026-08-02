#include "Store/DTOs/GetStore.hpp"
#include "Store/DTOs/SearchStore.hpp"
#include <Database.hpp>
#include <DataTable.hpp>
#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <Store/DTOs/CreateStore.hpp>
#include <Store/DTOs/UpdateStore.hpp>

namespace omnisphere::repositories
{
    class Store
    {
        public:
        explicit Store(std::shared_ptr<omnisphere::services::Database> database);
        ~Store();

        bool Create(const omnisphere::dtos::CreateStore &_store) const;
        bool Update(const omnisphere::dtos::UpdateStore &_store) const;
        omnisphere::types::DataTable Read(const omnisphere::dtos::GetStore &filter, const std::vector<std::string>& fields = {}) const;
        omnisphere::types::DataTable ReadAll(const std::vector<std::string>& fields = {}) const;
        omnisphere::types::DataTable Search(const std::vector<std::string>& fields, const omnisphere::dtos::SearchStore &filter) const;

        private:
        std::shared_ptr<omnisphere::services::Database> Database;

        int GetCurrentSequence() const;
        bool UpdateStoreSequence() const;
    };
} // namespace omnisphere::repositories
