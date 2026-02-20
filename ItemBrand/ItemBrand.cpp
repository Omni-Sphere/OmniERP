#include "ItemBrand.hpp"
#include "DTOs/CreateItemBrand.hpp"
#include "DTOs/GetItemBrand.hpp"
#include "DTOs/UpdateItemBrand.hpp"
#include "DataTable.hpp"
#include "Database.hpp"
#include "Models/ItemBrand.hpp"
#include "Repositories/ItemBrand.hpp"

namespace omnisphere::omnierp::services {

struct ItemBrand::Impl {
  std::shared_ptr<omnisphere::omnierp::repositories::ItemBrand>
      itemBrandRepository;
  explicit Impl(std::shared_ptr<omnisphere::omnidata::services::Database> db)
      : itemBrandRepository(
            std::make_shared<omnisphere::omnierp::repositories::ItemBrand>(
                db)) {}
};

ItemBrand::ItemBrand(
    std::shared_ptr<omnisphere::omnidata::services::Database> db)
    : pimpl(std::make_unique<Impl>(db)) {}

ItemBrand::~ItemBrand() = default;

bool ItemBrand::Add(
    const omnisphere::omnierp::dtos::CreateItemBrand &createItemBrand) const {
  try {
    return pimpl->itemBrandRepository->Create(createItemBrand);
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[AddItemBrand Exception] ") +
                             e.what());
  }
}

bool ItemBrand::Modify(
    const omnisphere::omnierp::dtos::UpdateItemBrand &updateItemBrand) const {
  try {
    return pimpl->itemBrandRepository->Update(updateItemBrand);
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[ModifyItemBrand Exception] ") +
                             e.what());
  }
}

std::vector<omnisphere::omnierp::models::ItemBrand> ItemBrand::GetAll() const {
  try {
    std::vector<omnisphere::omnierp::models::ItemBrand> itemBrands;
    omnisphere::omnidata::types::DataTable dataTable =
        pimpl->itemBrandRepository->ReadAll();

    for (int i = 0; i < dataTable.RowsCount(); i++)
      itemBrands.emplace_back(omnisphere::omnierp::models::ItemBrand{
          dataTable[i]["Entry"], dataTable[i]["Name"], dataTable[i]["Code"],
          dataTable[i]["CreatedBy"], dataTable[i]["CreateDate"],
          dataTable[i]["LastUpdatedBy"].GetOptional<int>(),
          dataTable[i]["UpdateDate"].GetOptional<std::string>()});

    return itemBrands;
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[GetAllItemBrands Exception] ") +
                             e.what());
  }
}

omnisphere::omnierp::models::ItemBrand ItemBrand::Get(
    const omnisphere::omnierp::dtos::GetItemBrand &getItemBrand) const {
  try {
    omnisphere::omnidata::types::DataTable dataTable =
        pimpl->itemBrandRepository->Read(getItemBrand);

    if (dataTable.RowsCount() == 0)
      throw std::runtime_error("No ItemBrands found");

    omnisphere::omnierp::models::ItemBrand itemBrand{
        dataTable[0]["Entry"],
        dataTable[0]["Code"],
        dataTable[0]["Name"],
        dataTable[0]["CreatedBy"],
        dataTable[0]["CreateDate"],
        dataTable[0]["LastUpdatedBy"].GetOptional<int>(),
        dataTable[0]["UpdateDate"].GetOptional<std::string>()};

    return itemBrand;
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[GetItemBrand Exception] ") +
                             e.what());
  }
}

} // namespace omnisphere::omnierp::services