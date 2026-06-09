#include <Database.hpp>
#include <DataTable.hpp>
#pragma once

#include <memory>
#include <Database.hpp>
#include <vector>

#include <ItemBrand/DTOs/CreateItemBrand.hpp>
#include <ItemBrand/DTOs/GetItemBrand.hpp>
#include <ItemBrand/DTOs/UpdateItemBrand.hpp>
#include <ItemBrand/Models/ItemBrand.hpp>

namespace omnisphere::services
{
    class ItemBrand
    {
        private:
        struct Impl;
        std::unique_ptr<Impl> pimpl;

        public:
        explicit ItemBrand(std::shared_ptr<omnisphere::services::Database> db);

        ~ItemBrand();

        omnisphere::models::ItemBrand
        Add(const omnisphere::dtos::CreateItemBrand &createItemBrand) const;

        omnisphere::models::ItemBrand
        Modify(const omnisphere::dtos::UpdateItemBrand &updateItemBrand) const;

        std::vector<omnisphere::models::ItemBrand> GetAll() const;

        omnisphere::models::ItemBrand
        Get(const omnisphere::dtos::GetItemBrand &getItemBrand) const;
    };
} // namespace omnisphere::services