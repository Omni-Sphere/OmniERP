#include <Database.hpp>
#include <DataTable.hpp>
#pragma once
#include <DataTable.hpp>

#include <optional>
#include <string>

#include <Item/DTOs/CreateItem.hpp>
#include <Item/DTOs/UpdateItem.hpp>
#include <Item/DTOs/ItemFilter.hpp>
#include <Item/DTOs/UpdateItem.hpp>
#include <Database.hpp>
#include <string>
#include <vector>

namespace omnisphere::repositories
{
    class Item
    {
        public:
        explicit Item(std::shared_ptr<omnisphere::services::Database> database);
        ~Item();

        bool Create(const omnisphere::dtos::CreateItem &_item) const;
        bool Update(const omnisphere::dtos::UpdateItem &_item) const;
        omnisphere::types::DataTable
        Search(const std::vector<std::string>& fields, const omnisphere::dtos::ItemFilter &_item) const;
        // Full read without field filtering (SELECT *)
        
        private:
        std::shared_ptr<omnisphere::services::Database> Database;

        int GetCurrentSequence() const;
        bool UpdateUserSequence() const;
    };
} // namespace omnisphere::repositories