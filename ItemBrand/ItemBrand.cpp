#include <Database.hpp>
#include <DataTable.hpp>
#include <DataMapper.hpp>
#include <ItemBrand/ItemBrand.hpp>
#include <ItemBrand/DTOs/CreateItemBrand.hpp>
#include <ItemBrand/DTOs/GetItemBrand.hpp>
#include <ItemBrand/DTOs/UpdateItemBrand.hpp>
#include <ItemBrand/Models/ItemBrand.hpp>
#include <ItemBrand/Repositories/ItemBrand.hpp>

namespace omnisphere::services
{
    struct ItemBrand::Impl
    {
        std::shared_ptr<omnisphere::repositories::ItemBrand> itemBrandRepository;
        explicit Impl(std::shared_ptr<omnisphere::data::Database> db)
            : itemBrandRepository(
                std::make_shared<omnisphere::repositories::ItemBrand>(db)) {}
    };

    ItemBrand::ItemBrand(std::shared_ptr<omnisphere::data::Database> db)
        : pimpl(std::make_unique<Impl>(db)) {}

    ItemBrand::~ItemBrand() = default;

    omnisphere::models::ItemBrand ItemBrand::Add(
        const std::vector<std::string>& fields,
        const omnisphere::dtos::CreateItemBrand &createItemBrand) const
    {
        try
        {
            if (pimpl->itemBrandRepository->Create(createItemBrand))
            {
                omnisphere::dtos::GetItemBrand getItemBrand;
                getItemBrand.Code = createItemBrand.Code;

                return Get(fields, getItemBrand);
            }
            else
            {
                throw std::runtime_error("Error creating item brand");
            }
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[AddItemBrand Exception] ") +
                                     e.what());
        }
    }

    omnisphere::models::ItemBrand ItemBrand::Modify(
        const std::vector<std::string>& fields,
        const omnisphere::dtos::UpdateItemBrand &updateItemBrand) const
    {
        try
        {
            if (pimpl->itemBrandRepository->Update(updateItemBrand))
            {
                omnisphere::dtos::GetItemBrand getItemBrand;
                getItemBrand.Code = updateItemBrand.Code;

                return Get(fields, getItemBrand);
            }
            else
            {
                throw std::runtime_error("Error updating item brand");
            }
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[ModifyItemBrand Exception] ") +
                                     e.what());
        }
    }

    std::vector<omnisphere::models::ItemBrand> ItemBrand::GetAll(const std::vector<std::string>& fields) const
    {
        try
        {
            std::vector<omnisphere::models::ItemBrand> itemBrands;
            omnisphere::types::DataTable dataTable =
            pimpl->itemBrandRepository->ReadAll(fields);

            for (int i = 0; i < dataTable.RowsCount(); i++)
                itemBrands.push_back(omnisphere::data::MapFromRow<omnisphere::models::ItemBrand>(dataTable[i]));

            return itemBrands;
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[GetAllItemBrands Exception] ") +
                                     e.what());
        }
    }

    omnisphere::models::ItemBrand
    ItemBrand::Get(const std::vector<std::string>& fields, const omnisphere::dtos::GetItemBrand &getItemBrand) const
    {
        try
        {
            omnisphere::types::DataTable dataTable =
            pimpl->itemBrandRepository->Search(fields, getItemBrand);

            if (dataTable.RowsCount() == 0)
                throw std::runtime_error("No ItemBrands found");

            return omnisphere::data::MapFromRow<omnisphere::models::ItemBrand>(dataTable[0]);
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[GetItemBrand Exception] ") +
                                     e.what());
        }
    }

} // namespace omnisphere::services