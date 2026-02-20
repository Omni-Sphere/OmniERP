#include "Item.hpp"

namespace omnisphere::omnierp::repositories {
Item::Item(std::shared_ptr<omnisphere::omnidata::services::Database> database)
    : Database(std::move(database)) {}

Item::~Item() = default;

bool Item::Create(const omnisphere::omnierp::dtos::CreateItem &item) const {
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

    const std::vector<omnisphere::omnidata::types::SQLParam> params = {
        omnisphere::omnidata::types::MakeSQLParam(GetCurrentSequence()),
        omnisphere::omnidata::types::MakeSQLParam(item.Code),
        omnisphere::omnidata::types::MakeSQLParam(item.Name),
        omnisphere::omnidata::types::MakeSQLParam(item.Description),
        omnisphere::omnidata::types::MakeSQLParam(item.Image),
        omnisphere::omnidata::types::MakeSQLParam(item.PurchaseItem),
        omnisphere::omnidata::types::MakeSQLParam(item.SellItem),
        omnisphere::omnidata::types::MakeSQLParam(item.InventoryItem),
        omnisphere::omnidata::types::MakeSQLParam(item.Price),
        omnisphere::omnidata::types::MakeSQLParam(item.Brand),
        omnisphere::omnidata::types::MakeSQLParam(item.Group),
        omnisphere::omnidata::types::MakeSQLParam(item.MinStock),
        omnisphere::omnidata::types::MakeSQLParam(item.MaxStock),
        omnisphere::omnidata::types::MakeSQLParam(item.MinOrder),
        omnisphere::omnidata::types::MakeSQLParam(item.MaxOrder),
        omnisphere::omnidata::types::MakeSQLParam(item.MinRequest),
        omnisphere::omnidata::types::MakeSQLParam(item.MaxRequest),
        omnisphere::omnidata::types::MakeSQLParam(item.CreatedBy),
        omnisphere::omnidata::types::MakeSQLParam(item.CreateDate)};

    if (!Database->RunPrepared(sQuery, params))
      throw std::runtime_error("Error creating item");

    UpdateUserSequence();

    return true;
  } catch (const std::exception &e) {
    Database->RollbackTransaction();
    throw std::runtime_error(std::string("[Create Exception]: ") + e.what());
  }
}

bool Item::Update(const omnisphere::omnierp::dtos::UpdateItem &item) const {
  try {
    std::string sQuery = "UPDATE Items SET ";
    std::vector<omnisphere::omnidata::types::SQLParam> params;

    if (item.Name.has_value()) {
      sQuery += "Name = ?, ";
      params.emplace_back(
          omnisphere::omnidata::types::MakeSQLParam(item.Name.value()));
    }

    if (item.Description.has_value()) {
      sQuery += "Description = ?, ";
      params.emplace_back(
          omnisphere::omnidata::types::MakeSQLParam(item.Description.value()));
    }

    if (item.Image.has_value()) {
      sQuery += "Image = ?, ";
      params.emplace_back(
          omnisphere::omnidata::types::MakeSQLParam(item.Image.value()));
    }

    if (item.PurchaseItem.has_value()) {
      sQuery += "PurchaseItem = ?, ";
      params.emplace_back(
          omnisphere::omnidata::types::MakeSQLParam(item.PurchaseItem.value()));
    }

    if (item.SellItem.has_value()) {
      sQuery += "SellItem = ?, ";
      params.emplace_back(
          omnisphere::omnidata::types::MakeSQLParam(item.SellItem.value()));
    }

    if (item.InventoryItem.has_value()) {
      sQuery += "InventoryItem = ?, ";
      params.emplace_back(omnisphere::omnidata::types::MakeSQLParam(
          item.InventoryItem.value()));
    }

    if (item.Price.has_value()) {
      sQuery += "Price = ?, ";
      params.emplace_back(
          omnisphere::omnidata::types::MakeSQLParam(item.Price.value()));
    }

    if (item.Brand.has_value()) {
      sQuery += "Brand = ?, ";
      params.emplace_back(
          omnisphere::omnidata::types::MakeSQLParam(item.Brand.value()));
    }

    if (item.Group.has_value()) {
      sQuery += "Group = ?, ";
      params.emplace_back(
          omnisphere::omnidata::types::MakeSQLParam(item.Group.value()));
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

omnisphere::omnidata::types::DataTable
Item::Read(const omnisphere::omnierp::dtos::SearchItems &filter) const {
  try {
    omnisphere::omnidata::types::DataTable datatable;

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

    const std::vector<omnisphere::omnidata::types::SQLParam> params = {
        omnisphere::omnidata::types::MakeSQLParam(filter.CodeEqualsTo),
        omnisphere::omnidata::types::MakeSQLParam(filter.CodeContains),
        omnisphere::omnidata::types::MakeSQLParam(filter.NameEqualsTo),
        omnisphere::omnidata::types::MakeSQLParam(filter.NameContains),
        omnisphere::omnidata::types::MakeSQLParam(filter.DescriptionEqualsTo),
        omnisphere::omnidata::types::MakeSQLParam(filter.DescriptionContains),
        omnisphere::omnidata::types::MakeSQLParam(filter.IsActive),
        omnisphere::omnidata::types::MakeSQLParam(filter.PurchaseItem),
        omnisphere::omnidata::types::MakeSQLParam(filter.SellItem),
        omnisphere::omnidata::types::MakeSQLParam(filter.InventoryItem),
        omnisphere::omnidata::types::MakeSQLParam(filter.Price),
        omnisphere::omnidata::types::MakeSQLParam(filter.OnHandEqualsTo),
        omnisphere::omnidata::types::MakeSQLParam(filter.OnHandGreaterThan),
        omnisphere::omnidata::types::MakeSQLParam(filter.OnHandLessThan),
        omnisphere::omnidata::types::MakeSQLParam(filter.OnRequestEqualsTo),
        omnisphere::omnidata::types::MakeSQLParam(filter.OnRequestGreaterThan),
        omnisphere::omnidata::types::MakeSQLParam(filter.OnRequestLessThan),
        omnisphere::omnidata::types::MakeSQLParam(filter.OnOrderEqualsTo),
        omnisphere::omnidata::types::MakeSQLParam(filter.OnOrderGreaterThan),
        omnisphere::omnidata::types::MakeSQLParam(filter.OnOrderLessThan),
        omnisphere::omnidata::types::MakeSQLParam(filter.MinStockEqualsTo),
        omnisphere::omnidata::types::MakeSQLParam(filter.MinStockGreaterThan),
        omnisphere::omnidata::types::MakeSQLParam(filter.MinStockLessThan),
        omnisphere::omnidata::types::MakeSQLParam(filter.MaxStockEqualsTo),
        omnisphere::omnidata::types::MakeSQLParam(filter.MaxStockGreaterThan),
        omnisphere::omnidata::types::MakeSQLParam(filter.MaxStockLessThan),
        omnisphere::omnidata::types::MakeSQLParam(filter.MinOrderEqualsTo),
        omnisphere::omnidata::types::MakeSQLParam(filter.MinOrderGreaterThan),
        omnisphere::omnidata::types::MakeSQLParam(filter.MinOrderLessThan),
        omnisphere::omnidata::types::MakeSQLParam(filter.MaxOrderEqualsTo),
        omnisphere::omnidata::types::MakeSQLParam(filter.MaxOrderGreaterThan),
        omnisphere::omnidata::types::MakeSQLParam(filter.MaxOrderLessThan),
        omnisphere::omnidata::types::MakeSQLParam(filter.MinRequestEqualsTo),
        omnisphere::omnidata::types::MakeSQLParam(filter.MinRequestGreaterThan),
        omnisphere::omnidata::types::MakeSQLParam(filter.MinRequestLessThan),
        omnisphere::omnidata::types::MakeSQLParam(filter.MaxReqeuestEqualsTo),
        omnisphere::omnidata::types::MakeSQLParam(filter.MaxRequestGreaterThan),
        omnisphere::omnidata::types::MakeSQLParam(filter.MaxRequestLessThan)};

    return datatable;
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[Read Exception]: ") + e.what());
  }
}

omnisphere::omnidata::types::DataTable
Item::Read(const omnisphere::omnierp::dtos::GetItem &filter) const {
  try {
    omnisphere::omnidata::types::DataTable datatable;

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

omnisphere::omnidata::types::DataTable Item::Read() const {
  try {
    omnisphere::omnidata::types::DataTable datatable;

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

    omnisphere::omnidata::types::DataTable data =
        Database->FetchResults(sQuery);

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
}; // namespace omnisphere::omnierp::repositories