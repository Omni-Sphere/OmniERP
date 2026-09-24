#include "Area/Area.hpp"
#include "Area/Repositories/Area.hpp"
#include <OmniData/DataTable.hpp>
#include <OmniData/Database.hpp>
#include <OmniData/DataMapper.hpp>
#include "Table/Repositories/Table.hpp"
#include <stdexcept>
#include <string>

namespace omnisphere::area {
Area::Area(std::shared_ptr<omnisphere::data::Database> database)
    : pImpl(std::make_unique<Impl>(database)) {}
Area::~Area() = default;

struct Area::Impl {
  std::shared_ptr<omnisphere::repositories::AreaRepository> areaRepository;
  std::shared_ptr<omnisphere::repositories::TableRepository> tableRepository;
  explicit Impl(std::shared_ptr<omnisphere::data::Database> database)
      : areaRepository(
            std::make_shared<omnisphere::repositories::AreaRepository>(
                database)),
        tableRepository(
            std::make_shared<omnisphere::repositories::TableRepository>(
                database)) {}
};

omnisphere::models::Area
Area::Add(const omnisphere::dtos::CreateArea &area) const {
  try {
    if (pImpl->areaRepository->Create(area)) {
      omnisphere::dtos::GetArea getArea;
      getArea.Code = area.Code;

      return Get(getArea);
    } else {
      throw std::runtime_error("Error adding area");
    }
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[AddArea Exception] ") + e.what());
  }
}
omnisphere::models::Area
Area::Modify(const omnisphere::dtos::UpdateArea &area) const {
  try {
    if (pImpl->areaRepository->Update(area)) {
      omnisphere::dtos::GetArea getArea;
      getArea.Code = area.Code;

      return Get(getArea);
    } else {
      throw std::runtime_error("Error updating area");
    }
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[ModifyArea Exception] ") + e.what());
  }
}

std::vector<omnisphere::models::Area> Area::GetAll(const std::vector<std::string>& fields) const {
  try {
    omnisphere::types::DataTable data = pImpl->areaRepository->ReadAll(fields);
    return omnisphere::types::DataTableToModels<omnisphere::models::Area>(data);
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[GetAllAreas Exception] ") +
                             e.what());
  }
}
omnisphere::models::Area
Area::Get(const omnisphere::dtos::GetArea &getArea, const std::vector<std::string>& fields) const {
  try {
    omnisphere::types::DataTable data = pImpl->areaRepository->Read(getArea, fields);

    if (data.RowsCount() == 0)
      throw std::runtime_error("Area doesn't exists");

    return omnisphere::types::FromDataRow<omnisphere::models::Area>(data[0]);
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[GetArea Exception] ") + e.what());
  }
}
std::vector<omnisphere::models::Area>
Area::Search(const omnisphere::dtos::GetArea &getArea, const std::vector<std::string>& fields) const {
  try {
    omnisphere::types::DataTable data = pImpl->areaRepository->Read(getArea, fields);
    return omnisphere::types::DataTableToModels<omnisphere::models::Area>(data);
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[SearchAreas Exception] ") +
                             e.what());
  }
}

bool Area::Remove(int entry) const {
  try {
    // Check for associated tables
    omnisphere::dtos::GetTable getTable;
    getTable.AreaEntry = entry;

    omnisphere::types::DataTable tables =
        pImpl->tableRepository->Read(getTable);

    if (tables.RowsCount() > 0) {
      throw std::runtime_error(
          "Cannot delete Area because it has associated Tables");
    }

    return pImpl->areaRepository->Delete(entry);
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[RemoveArea Exception] ") + e.what());
  }
}
} // namespace omnisphere::area
