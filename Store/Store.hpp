#include <Database.hpp>
#include <DataTable.hpp>
#pragma once

#include <memory>
#include <vector>
#include <optional>
#include <string>

#include <Store/DTOs/CreateStore.hpp>
#include <Store/DTOs/GetStore.hpp>
#include <Store/DTOs/UpdateStore.hpp>
#include <Store/Models/Store.hpp>

namespace omnisphere::services
{
    class Store
    {
        public:
        explicit Store(std::shared_ptr<omnisphere::services::Database> database);
        ~Store();

        std::optional<omnisphere::models::Store> Get(int entry, const std::vector<std::string>& fields = {}) const;
        std::optional<omnisphere::models::Store> GetByCode(const std::string& code, const std::vector<std::string>& fields = {}) const;
        std::vector<omnisphere::models::Store> GetAll(const std::vector<std::string>& fields = {}) const;
        std::vector<omnisphere::models::Store> Search(const std::vector<std::string>& fields, const omnisphere::dtos::GetStore &filter) const;
        bool Add(const omnisphere::dtos::CreateStore &_store) const;
        bool Modify(const omnisphere::dtos::UpdateStore &_store) const;

        private:
        struct Impl;
        std::unique_ptr<Impl> pimpl;
    };

} // namespace omnisphere::services
