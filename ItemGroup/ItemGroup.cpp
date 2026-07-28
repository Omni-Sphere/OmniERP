#include <Database.hpp>
#include <DataTable.hpp>
#include <DataMapper.hpp>
#include <ItemGroup/ItemGroup.hpp>
#include <ItemGroup/DTOs/CreateItemGroup.hpp>
#include <ItemGroup/DTOs/GetItemGroup.hpp>
#include <ItemGroup/DTOs/UpdateItemGroup.hpp>
#include <ItemGroup/Models/ItemGroup.hpp>
#include <ItemGroup/Repositories/ItemGroup.hpp>

namespace omnisphere::services
{
    struct ItemGroup::Impl
    {
        std::shared_ptr<omnisphere::repositories::ItemGroup> itemGroupRepository;
        explicit Impl(std::shared_ptr<omnisphere::services::Database> db)
            : itemGroupRepository(
                std::make_shared<omnisphere::repositories::ItemGroup>(db)) {}
    };

    ItemGroup::ItemGroup(std::shared_ptr<omnisphere::services::Database> db)
        : pimpl(std::make_unique<Impl>(db)) {}

    ItemGroup::~ItemGroup() = default;

    omnisphere::models::ItemGroup ItemGroup::Add(
        const std::vector<std::string>& fields,
        const omnisphere::dtos::CreateItemGroup &createItemGroup) const
    {
        try
        {
            if (pimpl->itemGroupRepository->Create(createItemGroup))
            {
                omnisphere::dtos::GetItemGroup getItemGroup;
                getItemGroup.Code = createItemGroup.Code;

                return Get(fields, getItemGroup);
            }
            else
            {
                throw std::runtime_error("Error creating item group");
            }
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[AddItemGroup Exception] ") +
                                     e.what());
        }
    }

    omnisphere::models::ItemGroup ItemGroup::Modify(
        const std::vector<std::string>& fields,
        const omnisphere::dtos::UpdateItemGroup &updateItemGroup) const
    {
        try
        {
            if (pimpl->itemGroupRepository->Update(updateItemGroup))
            {
                omnisphere::dtos::GetItemGroup getItemGroup;
                getItemGroup.Code = updateItemGroup.Code;

                return Get(fields, getItemGroup);
            }
            else
            {
                throw std::runtime_error("Error updating item group");
            }
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[ModifyItemGroup Exception] ") +
                                     e.what());
        }
    }

    std::vector<omnisphere::models::ItemGroup> ItemGroup::GetAll(const std::vector<std::string>& fields) const
    {
        try
        {
            std::vector<omnisphere::models::ItemGroup> itemGroups;
            omnisphere::types::DataTable dataTable =
            pimpl->itemGroupRepository->ReadAll(fields);

            for (int i = 0; i < dataTable.RowsCount(); i++)
                itemGroups.push_back(omnisphere::data::MapFromRow<omnisphere::models::ItemGroup>(dataTable[i]));

            return itemGroups;
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[GetAllItemGroups Exception] ") +
                                     e.what());
        }
    }

    omnisphere::models::ItemGroup
    ItemGroup::Get(const std::vector<std::string>& fields, const omnisphere::dtos::GetItemGroup &getItemGroup) const
    {
        try
        {
            omnisphere::types::DataTable dataTable =
            pimpl->itemGroupRepository->Search(fields, getItemGroup);

            if (dataTable.RowsCount() == 0)
                throw std::runtime_error("No ItemGroups found");

            return omnisphere::data::MapFromRow<omnisphere::models::ItemGroup>(dataTable[0]);
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[GetItemGroup Exception] ") +
                                     e.what());
        }
    }
} // namespace omnisphere::services