#include "Item.hpp"

namespace omnisphere::repositories {
Item::Item(std::shared_ptr<omnisphere::services::Database> database)
    : Database(std::move(database)) {}

Item::~Item() = default;

bool Item::Create(const omnisphere::dtos::CreateItem &item) const {
  try {
    std::string sQuery =
        "INSERT INTO Items ("
        "[ItemEntry], "
        "[Code], "
        "[Name], "
        "Description, "
        "Image, "
        "PurchaseItem, "
        "SellItem, "
        "InventoryItem, "
        "Price, "
        "BrandEntry, "
        "GroupEntry, "
        "MinStock, "
        "MaxStock, "
        "MinOrder, "
        "MaxOrder, "
        "MinRequest, "
        "MaxRequest, "
        "CreatedBy, "
        "CreateDate"
        ")"
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";

    const std::vector<omnisphere::types::SQLParam> params = {
        omnisphere::types::MakeSQLParam(GetCurrentSequence()),
        omnisphere::types::MakeSQLParam(item.Code),
        omnisphere::types::MakeSQLParam(item.Name),
        omnisphere::types::MakeSQLParam(item.Description),
        omnisphere::types::MakeSQLParam(item.Image),
        omnisphere::types::MakeSQLParam(item.PurchaseItem),
        omnisphere::types::MakeSQLParam(item.SellItem),
        omnisphere::types::MakeSQLParam(item.InventoryItem),
        omnisphere::types::MakeSQLParam(item.Price),
        omnisphere::types::MakeSQLParam(item.Brand),
        omnisphere::types::MakeSQLParam(item.Group),
        omnisphere::types::MakeSQLParam(item.MinStock),
        omnisphere::types::MakeSQLParam(item.MaxStock),
        omnisphere::types::MakeSQLParam(item.MinOrder),
        omnisphere::types::MakeSQLParam(item.MaxOrder),
        omnisphere::types::MakeSQLParam(item.MinRequest),
        omnisphere::types::MakeSQLParam(item.MaxRequest),
        omnisphere::types::MakeSQLParam(item.CreatedBy),
        omnisphere::types::MakeSQLParam(item.CreateDate)};

    if (!Database->RunPrepared(sQuery, params))
      throw std::runtime_error("Error creating item");

    UpdateUserSequence();

    return true;
  } catch (const std::exception &e) {
    Database->RollbackTransaction();
    throw std::runtime_error(std::string("[Create Exception]: ") + e.what());
  }
}

bool Item::Update(const omnisphere::dtos::UpdateItem &item) const {
  try {
    std::string sQuery = "UPDATE Items SET ";
    std::vector<omnisphere::types::SQLParam> params;

    if (item.Name.has_value()) {
      sQuery += "Name = ?, ";
      params.emplace_back(omnisphere::types::MakeSQLParam(item.Name.value()));
    }

    if (item.Description.has_value()) {
      sQuery += "Description = ?, ";
      params.emplace_back(
          omnisphere::types::MakeSQLParam(item.Description.value()));
    }

    if (item.Image.has_value()) {
      sQuery += "Image = ?, ";
      params.emplace_back(omnisphere::types::MakeSQLParam(item.Image.value()));
    }

    if (item.PurchaseItem.has_value()) {
      sQuery += "PurchaseItem = ?, ";
      params.emplace_back(
          omnisphere::types::MakeSQLParam(item.PurchaseItem.value()));
    }

    if (item.SellItem.has_value()) {
      sQuery += "SellItem = ?, ";
      params.emplace_back(
          omnisphere::types::MakeSQLParam(item.SellItem.value()));
    }

    if (item.InventoryItem.has_value()) {
      sQuery += "InventoryItem = ?, ";
      params.emplace_back(
          omnisphere::types::MakeSQLParam(item.InventoryItem.value()));
    }

    if (item.Price.has_value()) {
      sQuery += "Price = ?, ";
      params.emplace_back(omnisphere::types::MakeSQLParam(item.Price.value()));
    }

    if (item.Brand.has_value()) {
      sQuery += "Brand = ?, ";
      params.emplace_back(omnisphere::types::MakeSQLParam(item.Brand.value()));
    }

    if (item.Group.has_value()) {
      sQuery += "Group = ?, ";
      params.emplace_back(omnisphere::types::MakeSQLParam(item.Group.value()));
    }

    sQuery += "WHERE ItemEntry = ?";

    if (!Database->RunPrepared(sQuery, params))
      throw std::runtime_error("Error updating item");

    return true;
  } catch (const std::exception &e) {
    Database->RollbackTransaction();
    throw std::runtime_error(std::string("[Update Exception]: ") + e.what());
  }
}

omnisphere::types::DataTable
Item::Read(const omnisphere::dtos::SearchItems &filter) const {
  try {
    omnisphere::types::DataTable datatable;

    std::string sQuery = "SELECT "
                         "[ItemEntry], "
                         "[Code], "
                         "[Name], "
                         "Description, "
                         "Image, "
                         "IsActive, "
                         "PurchaseItem, "
                         "SellItem, "
                         "InventoryItem, "
                         "Price, "
                         "BrandEntry Brand, "
                         "GroupEntry [Group],"
                         "OnHand, "
                         "OnOrder, "
                         "OnRequest, "
                         "MinStock, "
                         "MaxStock, "
                         "MinOrder, "
                         "MaxOrder, "
                         "MinRequest, "
                         "MaxRequest, "
                         "CreatedBy, "
                         "CreateDate, "
                         "LastUpdatedBy, "
                         "UpdateDate "
                         "FROM Items WHERE ";

    const std::vector<omnisphere::types::SQLParam> params = {
        omnisphere::types::MakeSQLParam(filter.CodeEqualsTo),
        omnisphere::types::MakeSQLParam(filter.CodeContains),
        omnisphere::types::MakeSQLParam(filter.NameEqualsTo),
        omnisphere::types::MakeSQLParam(filter.NameContains),
        omnisphere::types::MakeSQLParam(filter.DescriptionEqualsTo),
        omnisphere::types::MakeSQLParam(filter.DescriptionContains),
        omnisphere::types::MakeSQLParam(filter.IsActive),
        omnisphere::types::MakeSQLParam(filter.PurchaseItem),
        omnisphere::types::MakeSQLParam(filter.SellItem),
        omnisphere::types::MakeSQLParam(filter.InventoryItem),
        omnisphere::types::MakeSQLParam(filter.Price),
        omnisphere::types::MakeSQLParam(filter.OnHandEqualsTo),
        omnisphere::types::MakeSQLParam(filter.OnHandGreaterThan),
        omnisphere::types::MakeSQLParam(filter.OnHandLessThan),
        omnisphere::types::MakeSQLParam(filter.OnRequestEqualsTo),
        omnisphere::types::MakeSQLParam(filter.OnRequestGreaterThan),
        omnisphere::types::MakeSQLParam(filter.OnRequestLessThan),
        omnisphere::types::MakeSQLParam(filter.OnOrderEqualsTo),
        omnisphere::types::MakeSQLParam(filter.OnOrderGreaterThan),
        omnisphere::types::MakeSQLParam(filter.OnOrderLessThan),
        omnisphere::types::MakeSQLParam(filter.MinStockEqualsTo),
        omnisphere::types::MakeSQLParam(filter.MinStockGreaterThan),
        omnisphere::types::MakeSQLParam(filter.MinStockLessThan),
        omnisphere::types::MakeSQLParam(filter.MaxStockEqualsTo),
        omnisphere::types::MakeSQLParam(filter.MaxStockGreaterThan),
        omnisphere::types::MakeSQLParam(filter.MaxStockLessThan),
        omnisphere::types::MakeSQLParam(filter.MinOrderEqualsTo),
        omnisphere::types::MakeSQLParam(filter.MinOrderGreaterThan),
        omnisphere::types::MakeSQLParam(filter.MinOrderLessThan),
        omnisphere::types::MakeSQLParam(filter.MaxOrderEqualsTo),
        omnisphere::types::MakeSQLParam(filter.MaxOrderGreaterThan),
        omnisphere::types::MakeSQLParam(filter.MaxOrderLessThan),
        omnisphere::types::MakeSQLParam(filter.MinRequestEqualsTo),
        omnisphere::types::MakeSQLParam(filter.MinRequestGreaterThan),
        omnisphere::types::MakeSQLParam(filter.MinRequestLessThan),
        omnisphere::types::MakeSQLParam(filter.MaxReqeuestEqualsTo),
        omnisphere::types::MakeSQLParam(filter.MaxRequestGreaterThan),
        omnisphere::types::MakeSQLParam(filter.MaxRequestLessThan)};

    return datatable;
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[Read Exception]: ") + e.what());
  }
}

omnisphere::types::DataTable
Item::Read(const omnisphere::dtos::GetItem &filter) const {
  try {
    omnisphere::types::DataTable datatable;

    std::string sQuery = "SELECT "
                         "[ItemEntry], "
                         "[Code], "
                         "[Name], "
                         "Description, "
                         "Image, "
                         "IsActive, "
                         "'N' StockByWarehouse, "
                         "PurchaseItem, "
                         "SellItem, "
                         "InventoryItem, "
                         "Price, "
                         "BrandEntry Brand, "
                         "GroupEntry [Group], "
                         "OnHand, "
                         "OnOrder, "
                         "OnRequest, "
                         "MinStock, "
                         "MaxStock, "
                         "MinOrder, "
                         "MaxOrder, "
                         "MinRequest, "
                         "MaxRequest, "
                         "CreatedBy, "
                         "CreateDate, "
                         "LastUpdatedBy, "
                         "UpdateDate "
                         "FROM Items WHERE ";

    if (filter.Code.has_value()) {
      sQuery += "[Code] = ?";
      datatable = Database->FetchPrepared(sQuery, filter.Code.value());
    }

    if (filter.Name.has_value()) {
      sQuery += "[Name] = ?";
      datatable = Database->FetchPrepared(sQuery, filter.Name.value());
    }

    return datatable;
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[Read Exception]: ") + e.what());
  }
}

omnisphere::types::DataTable Item::Read() const {
  try {
    omnisphere::types::DataTable datatable;

    std::string sQuery = "SELECT "
                         "[ItemEntry], "
                         "[Code], "
                         "[Name], "
                         "Description, "
                         "Image, "
                         "IsActive, "
                         "PurchaseItem, "
                         "SellItem, "
                         "InventoryItem, "
                         "Price, "
                         "BrandEntry Brand, "
                         "GroupEntry [Group], "
                         "OnHand, "
                         "OnOrder, "
                         "OnRequest, "
                         "MinStock, "
                         "MaxStock, "
                         "MinOrder, "
                         "MaxOrder, "
                         "MinRequest, "
                         "MaxRequest, "
                         "CreatedBy, "
                         "CreateDate, "
                         "LastUpdatedBy, "
                         "UpdateDate "
                         "FROM Items";
    datatable = Database->FetchResults(sQuery);

    return datatable;
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[Read Exception]: ") + e.what());
  }
}

int Item::GetCurrentSequence() const {
  try {
    const std::string sQuery = "SELECT ISNULL(ItemSequence, 0) + 1 "
                               "ItemSequence FROM Sequences WHERE SeqEntry = 1";

    omnisphere::types::DataTable data = Database->FetchResults(sQuery);

    if (data.RowsCount() == 1)
      return data[0]["ItemSequence"];
    else
      return 0;
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[GetCurrentSequence Exception] ") +
                             " " + e.what());
  }
};

bool Item::UpdateUserSequence() const {
  try {
    const std::string sQuery =
        "UPDATE Sequences SET ItemSequence = ISNULL(ItemSequence,0) + 1";

    if (!Database->RunStatement(sQuery))
      return false;

    return true;
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[UpdateUserSequence Exception] ") +
                             " " + e.what());
  }
};
}; // namespace omnisphere::repositories