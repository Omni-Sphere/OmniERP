#include <Database.hpp>
#include <DataTable.hpp>
#pragma once

#include <memory>
#include <Database.hpp>
#include <vector>

#include <Database.hpp>

#include <Item/DTOs/CreateItem.hpp>
#include <Item/DTOs/UpdateItem.hpp>
#include <Item/DTOs/ItemFilter.hpp>
#include <Item/Models/Item.hpp>
#include <DataTable.hpp>
#include <string>
#include <vector>

namespace omnisphere::services
{
    class Item
    {
        public:
        explicit Item(std::shared_ptr<omnisphere::data::Database> database);

        ~Item();

        omnisphere::models::Item Get(const std::vector<std::string>& fields, const omnisphere::dtos::ItemFilter &_item) const;
        std::vector<omnisphere::models::Item> GetAll(const std::vector<std::string>& fields) const;
        std::vector<omnisphere::models::Item>
        Search(const std::vector<std::string>& fields, const omnisphere::dtos::ItemFilter &_item) const;

        omnisphere::models::Item Add(const std::vector<std::string>& fields, const omnisphere::dtos::CreateItem &_item) const;

        omnisphere::models::Item
        Modify(const std::vector<std::string>& fields, const omnisphere::dtos::UpdateItem &_item) const;

        private:
        struct Impl;
        std::unique_ptr<Impl> pimpl;
    };
} // namespace omnisphere::services