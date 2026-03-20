#include "ItemGroup.hpp"
#include "DataTable.hpp"
#include "Database.hpp"
#include "Models/ItemGroup.hpp"
#include "Repositories/ItemGroup.hpp"

namespace omnisphere::services {

struct ItemGroup::Impl {
  std::shared_ptr<omnisphere::repositories::ItemGroup> itemGroupRepository;
  explicit Impl(std::shared_ptr<omnisphere::services::Database> db)
      : itemGroupRepository(
            std::make_shared<omnisphere::repositories::ItemGroup>(db)) {}
};

ItemGroup::ItemGroup(std::shared_ptr<omnisphere::services::Database> db)
    : pimpl(std::make_unique<Impl>(db)) {}

ItemGroup::~ItemGroup() = default;

omnisphere::models::ItemGroup ItemGroup::Add(
    const omnisphere::dtos::CreateItemGroup &createItemGroup) const {
  try {
    if (pimpl->itemGroupRepository->Create(createItemGroup)) {
      omnisphere::dtos::GetItemGroup getItemGroup;
      getItemGroup.Code = createItemGroup.Code;
      return Get(getItemGroup);
    } else {
      throw std::runtime_error("Error creating item group");
    }
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[AddItemGroup Exception] ") +
                             e.what());
  }
}

omnisphere::models::ItemGroup ItemGroup::Modify(
    const omnisphere::dtos::UpdateItemGroup &updateItemGroup) const {
  try {
    if (pimpl->itemGroupRepository->Update(updateItemGroup)) {
      omnisphere::dtos::GetItemGroup getItemGroup;
      getItemGroup.Code = updateItemGroup.Code;
      return Get(getItemGroup);
    } else {
      throw std::runtime_error("Error updating item group");
    }
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[ModifyItemGroup Exception] ") +
                             e.what());
  }
}

std::vector<omnisphere::models::ItemGroup> ItemGroup::GetAll() const {
  try {
    std::vector<omnisphere::models::ItemGroup> itemGroups;
    omnisphere::types::DataTable dataTable =
        pimpl->itemGroupRepository->ReadAll();

    for (int i = 0; i < dataTable.RowsCount(); i++)
      itemGroups.emplace_back(omnisphere::models::ItemGroup{
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

omnisphere::models::ItemGroup
ItemGroup::Get(const omnisphere::dtos::GetItemGroup &getItemGroup) const {
  try {
    omnisphere::types::DataTable dataTable =
        pimpl->itemGroupRepository->Read(getItemGroup);

    if (dataTable.RowsCount() == 0)
      throw std::runtime_error("No ItemGroups found");

    omnisphere::models::ItemGroup itemGroup{
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
} // namespace omnisphere::services