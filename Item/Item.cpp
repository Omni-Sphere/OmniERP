#include <Database.hpp>
#include <DataTable.hpp>
#include <DataTable.hpp>
#include <Database.hpp>
#include <DataTable.hpp>
#include <Item/Item.hpp>
#include <Item/DTOs/CreateItem.hpp>
#include <Item/DTOs/GetItem.hpp>
#include <Item/DTOs/SearchItems.hpp>
#include <Item/Repositories/Item.hpp>
#include <DataMapper.hpp>

namespace omnisphere::services
{
    // Define the Impl struct for Pimpl idiom
    struct Item::Impl
    {
        std::shared_ptr<omnisphere::repositories::Item> item;
        explicit Impl(std::shared_ptr<omnisphere::services::Database> database)
            : item(std::make_shared<omnisphere::repositories::Item>(database)) {}
    };

    // Update constructor to initialize pimpl
    Item::Item(std::shared_ptr<omnisphere::services::Database> database)
        : pimpl(std::make_unique<Impl>(database)) {}

    // Define destructor
    Item::~Item() = default;

    omnisphere::models::Item
    Item::Get(const std::vector<std::string>& fields, const omnisphere::dtos::ItemFilter &_item) const
    {
        try
        {
            std::vector<omnisphere::models::Item> items = Search(fields, _item);
            if (items.empty())
                throw std::runtime_error("Item doesn't exists");
            return items.front();
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[GetItem Exception] ") + e.what());
        }
    };

    std::vector<omnisphere::models::Item> Item::GetAll(const std::vector<std::string>& fields) const
    {
        try
        {
            return Search(fields, omnisphere::dtos::ItemFilter());
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[GetAllItems Exception] ") + e.what());
        }
    }

    std::vector<omnisphere::models::Item>
    Item::Search(const std::vector<std::string>& fields, const omnisphere::dtos::ItemFilter &_item) const
    {
        try
        {
            std::vector<omnisphere::models::Item> items;
            omnisphere::types::DataTable data = pimpl->item->Read(fields, _item);

            for (int i = 0; i < data.RowsCount(); i++)
            {
                omnisphere::models::Item item = omnisphere::data::MapFromRow<omnisphere::models::Item>(data[i]);

                if (data[i].HasColumn("Brand_Entry")) {
                    item.BrandObj = std::make_shared<omnisphere::models::ItemBrand>(
                        omnisphere::data::MapFromRow<omnisphere::models::ItemBrand>(data[i], "Brand_")
                    );
                    item.BrandObj->IsActive = true;
                }

                if (data[i].HasColumn("Group_Entry")) {
                    item.GroupObj = std::make_shared<omnisphere::models::ItemGroup>(
                        omnisphere::data::MapFromRow<omnisphere::models::ItemGroup>(data[i], "Group_")
                    );
                    item.GroupObj->IsActive = true;
                }

                items.push_back(item);
            }

            return items;
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[SearchItems Exception] ") +
                                     e.what());
        }
    }

    omnisphere::models::Item
    Item::Add(const std::vector<std::string>& fields, const omnisphere::dtos::CreateItem &_item) const
    {
        try
        {
            if (pimpl->item->Create(_item))
            {
                omnisphere::dtos::ItemFilter filter;
                filter.Code = omnisphere::dtos::StringFilter();
                filter.Code->eq = _item.Code;

                return Get(fields, filter);
            }
            else
                throw std::runtime_error("Error creating item ");
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[AddItem Exception] ") + e.what());
        }
    };

    omnisphere::models::Item
    Item::Modify(const std::vector<std::string>& fields, const omnisphere::dtos::UpdateItem &_item) const
    {
        try
        {
            if (pimpl->item->Update(_item))
            {
                omnisphere::dtos::ItemFilter filter;
                filter.Code = omnisphere::dtos::StringFilter();
                filter.Code->eq = _item.Code;

                return Get(fields, filter);
            }
            else
                throw std::runtime_error("Error updating item ");
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[Item::Modify Exception] ") +
                                     e.what());
        }
    };
} // namespace omnisphere::services
