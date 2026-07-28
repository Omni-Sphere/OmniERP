#include <Database.hpp>
#include <DataTable.hpp>
#pragma once
#include <DataTable.hpp>

#include <ItemGroup/DTOs/CreateItemGroup.hpp>
#include <ItemGroup/DTOs/GetItemGroup.hpp>
#include <ItemGroup/DTOs/UpdateItemGroup.hpp>
#include <Database.hpp>

#include <stdexcept>
#include <string>
#include <vector>

namespace omnisphere::repositories
{
    class ItemGroup
    {
        private:
        std::shared_ptr<omnisphere::services::Database> database;
        int GetCurrentSequence() const;
        bool UpdateItemGroupsSequence() const;

        public:
        explicit ItemGroup(std::shared_ptr<omnisphere::services::Database> Database);
        ~ItemGroup() {};

        bool Create(const omnisphere::dtos::CreateItemGroup &createItemGroup) const;
        bool Update(const omnisphere::dtos::UpdateItemGroup &updateItemGroup) const;
        omnisphere::types::DataTable ReadAll(const std::vector<std::string>& fields = {}) const;
        omnisphere::types::DataTable Search(const std::vector<std::string>& fields, const omnisphere::dtos::GetItemGroup &itemGroup) const;
    };
} // namespace omnisphere::repositories