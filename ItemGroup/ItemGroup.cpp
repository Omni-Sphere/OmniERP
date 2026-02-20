#include "ItemGroup.hpp"
#include "DataTable.hpp"
#include "Database.hpp"
#include "Models/ItemBrand.hpp"
#include "Models/ItemGroup.hpp"
#include "Repositories/ItemGroup.hpp"

namespace omnisphere::omnierp::services {

struct ItemGroup::Impl {
  std::shared_ptr<omnisphere::omnierp::repositories::ItemGroup>
      itemGroupRepository;
  explicit Impl(std::shared_ptr<omnisphere::omnidata::services::Database> db)
      : itemGroupRepository(
            std::make_shared<omnisphere::omnierp::repositories::ItemGroup>(
                db)) {}
};

ItemGroup::ItemGroup(
    std::shared_ptr<omnisphere::omnidata::services::Database> db)
    : pimpl(std::make_unique<Impl>(db)) {}

ItemGroup::~ItemGroup() = default;

bool ItemGroup::Add(
    const omnisphere::omnierp::dtos::CreateItemGroup &createItemGroup) const {
  try {
    return pimpl->itemGroupRepository->Create(createItemGroup);
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[AddItemGroup Exception] ") +
                             e.what());
  }
}

bool ItemGroup::Modify(
    const omnisphere::omnierp::dtos::UpdateItemGroup &updateItemGroup) const {
  try {
    return pimpl->itemGroupRepository->Update(updateItemGroup);
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[ModifyItemGroup Exception] ") +
                             e.what());
  }
}

std::vector<omnisphere::omnierp::models::ItemGroup> ItemGroup::GetAll() const {
  try {
    std::vector<omnisphere::omnierp::models::ItemGroup> itemGroups;
    omnisphere::omnidata::types::DataTable dataTable =
        pimpl->itemGroupRepository->ReadAll();

    for (int i = 0; i < dataTable.RowsCount(); i++)
      itemGroups.emplace_back(omnisphere::omnierp::models::ItemGroup{
          dataTable[i]["Entry"], dataTable[i]["Code"], dataTable[i]["Name"],
          dataTable[i]["CreatedBy"], dataTable[i]["CreateDate"],
          dataTable[i]["LastUpdatedBy"].GetOptional<int>(),
          dataTable[i]["UpdateDate"].GetOptional<std::string>()});

    return itemGroups;
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[GetAllItemGroups Exception] ") +
                             e.what());
  }
}

omnisphere::omnierp::models::ItemGroup ItemGroup::Get(
    const omnisphere::omnierp::dtos::GetItemGroup &getItemGroup) const {
  try {
    omnisphere::omnidata::types::DataTable dataTable =
        pimpl->itemGroupRepository->Read(getItemGroup);

    if (dataTable.RowsCount() == 0)
      throw std::runtime_error("No ItemGroups found");

    omnisphere::omnierp::models::ItemGroup itemGroup{
        dataTable[0]["Entry"],
        dataTable[0]["Code"],
        dataTable[0]["Name"],
        dataTable[0]["CreatedBy"],
        dataTable[0]["CreateDate"],
        dataTable[0]["LastUpdatedBy"].GetOptional<int>(),
        dataTable[0]["UpdateDate"].GetOptional<std::string>()};

    return itemGroup;
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[GetItemGroup Exception] ") +
                             e.what());
  }
}
} // namespace omnisphere::omnierp::services