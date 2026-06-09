#include <Database.hpp>
#include <DataTable.hpp>
#pragma once
#include <DataTable.hpp>

#include <ItemBrand/DTOs/CreateItemBrand.hpp>
#include <ItemBrand/DTOs/GetItemBrand.hpp>
#include <ItemBrand/DTOs/UpdateItemBrand.hpp>
#include <Database.hpp>

namespace omnisphere::repositories
{
    class ItemBrand
    {
        private:
        std::shared_ptr<omnisphere::services::Database> database;
        int GetCurrentSequence() const;
        bool UpdateItemBrandsSequence() const;

        public:
        explicit ItemBrand(std::shared_ptr<omnisphere::services::Database> Database);
        ~ItemBrand( {};
                   bool Create(const omnisphere::dtos::CreateItemBrand &createItemBrand) const;
                   bool Update(const omnisphere::dtos::UpdateItemBrand &updateItemBrand) const;
                   omnisphere::types::DataTable ReadAll() const;
                   omnisphere::types::DataTable Read(const omnisphere::dtos::GetItemBrand) const;
    };
} // namespace omnisphere::repositories