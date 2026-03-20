#include "Item.hpp" // Keep the original header for the service class
#include "DTOs/CreateItem.hpp"
#include "DTOs/GetItem.hpp"
#include "DTOs/SearchItems.hpp"
#include "DataTable.hpp"
#include "Database.hpp"          // Add new include for database
#include "Repositories/Item.hpp" // Add new include for repository

namespace omnisphere::services {
// Define the Impl struct for Pimpl idiom
struct Item::Impl {
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
Item::Get(const omnisphere::dtos::GetItem &_item) const {
  try {
    // Use pimpl to access the repository
    omnisphere::types::DataTable data = pimpl->item->Read(_item);

    if (data.RowsCount() == 0)
      throw std::runtime_error("Item doesn't exists");

    omnisphere::models::Item item(
        data[0]["ItemEntry"], data[0]["Code"], data[0]["Name"],
        data[0]["Description"].GetOptional<std::string>(),
        data[0]["Image"].GetOptional<std::string>(), data[0]["IsActive"],
        data[0]["PurchaseItem"], data[0]["SellItem"], data[0]["InventoryItem"],
        data[0]["Price"], data[0]["Brand"].GetOptional<int>(),
        data[0]["Group"].GetOptional<int>(), data[0]["OnHand"],
        data[0]["OnOrder"].GetOptional<double>(),
        data[0]["OnRequest"].GetOptional<double>(),
        data[0]["MinStock"].GetOptional<double>(),
        data[0]["MaxStock"].GetOptional<double>(),
        data[0]["MinOrder"].GetOptional<double>(),
        data[0]["MaxOrder"].GetOptional<double>(),
        data[0]["MinRequest"].GetOptional<double>(),
        data[0]["MaxRequest"].GetOptional<double>(), data[0]["CreatedBy"],
        data[0]["CreateDate"], data[0]["LastUpdatedBy"].GetOptional<int>(),
        data[0]["UpdateDate"].GetOptional<std::string>());

    return item;
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[GetItem Exception] ") + e.what());
  }
};

std::vector<omnisphere::models::Item> Item::GetAll() const {
  try {
    std::vector<omnisphere::models::Item> items;
    // Use pimpl to access the repository and call Read (alias for ReadAll)
    omnisphere::types::DataTable data = pimpl->item->Read();

    for (int i = 0; i < data.RowsCount(); i++) {
      items.emplace_back(data[i]["ItemEntry"], data[i]["Code"], data[i]["Name"],
                         data[i]["Description"].GetOptional<std::string>(),
                         data[i]["Image"].GetOptional<std::string>(),
                         data[i]["IsActive"], data[i]["PurchaseItem"],
                         data[i]["SellItem"], data[i]["InventoryItem"],
                         data[i]["Price"], data[i]["Brand"].GetOptional<int>(),
                         data[i]["Group"].GetOptional<int>(), data[i]["OnHand"],
                         data[i]["OnOrder"].GetOptional<double>(),
                         data[i]["OnRequest"].GetOptional<double>(),
                         data[i]["MinStock"].GetOptional<double>(),
                         data[i]["MaxStock"].GetOptional<double>(),
                         data[i]["MinOrder"].GetOptional<double>(),
                         data[i]["MaxOrder"].GetOptional<double>(),
                         data[i]["MinRequest"].GetOptional<double>(),
                         data[i]["MaxRequest"].GetOptional<double>(),
                         data[i]["CreatedBy"], data[i]["CreateDate"],
                         data[i]["LastUpdatedBy"].GetOptional<int>(),
                         data[i]["UpdateDate"].GetOptional<std::string>());
    }

    return items;
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[GetAllItems Exception] ") +
                             e.what());
  }
}

std::vector<omnisphere::models::Item>
Item::Search(omnisphere::dtos::SearchItems &_item) const {
  try {
    std::vector<omnisphere::models::Item> items;
    // Use pimpl to access the repository
    omnisphere::types::DataTable data = pimpl->item->Read(_item);

    for (int i = 0; i < data.RowsCount(); i++) {
      items.emplace_back(data[i]["ItemEntry"], data[i]["Code"], data[i]["Name"],
                         data[i]["Description"].GetOptional<std::string>(),
                         data[i]["Image"].GetOptional<std::string>(),
                         data[i]["IsActive"], data[i]["PurchaseItem"],
                         data[i]["SellItem"], data[i]["InventoryItem"],
                         data[i]["Price"], data[i]["Brand"].GetOptional<int>(),
                         data[i]["Group"].GetOptional<int>(), data[i]["OnHand"],
                         data[i]["OnOrder"].GetOptional<double>(),
                         data[i]["OnRequest"].GetOptional<double>(),
                         data[i]["MinStock"].GetOptional<double>(),
                         data[i]["MaxStock"].GetOptional<double>(),
                         data[i]["MinOrder"].GetOptional<double>(),
                         data[i]["MaxOrder"].GetOptional<double>(),
                         data[i]["MinRequest"].GetOptional<double>(),
                         data[i]["MaxRequest"].GetOptional<double>(),
                         data[i]["CreatedBy"], data[i]["CreateDate"],
                         data[i]["LastUpdatedBy"].GetOptional<int>(),
                         data[i]["UpdateDate"].GetOptional<std::string>());
    }

    return items;
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[SearchItems Exception] ") +
                             e.what());
  }
}

omnisphere::models::Item
Item::Add(const omnisphere::dtos::CreateItem &_item) const {
  try {
    if (pimpl->item->Create(_item)) {
      omnisphere::dtos::GetItem getItem;
      getItem.Code = _item.Code;
      
      return Get(getItem);
    } else
      throw std::runtime_error("Error creating item ");
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[AddItem Exception] ") + e.what());
  }
};

omnisphere::models::Item
Item::Modify(const omnisphere::dtos::UpdateItem &_item) const {
  try {
    if (pimpl->item->Update(_item)) {
      omnisphere::dtos::GetItem getItem;
      getItem.Code = _item.Code;

      return Get(getItem);
    } else
      throw std::runtime_error("Error updating item ");
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[Item::Modify Exception] ") +
                             e.what());
  }
};
} // namespace omnisphere::services