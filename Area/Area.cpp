#include "Area.hpp"
#include "DataTable.hpp"
#include "Repositories/Area.hpp"
#include "Repositories/Table.hpp"
#include <stdexcept>
#include <string>

namespace omnisphere::area {
Area::Area(std::shared_ptr<omnisphere::services::Database> database)
    : pImpl(std::make_unique<Impl>(database)) {}
Area::~Area() = default;

struct Area::Impl {
  std::shared_ptr<omnisphere::repositories::AreaRepository> areaRepository;
  std::shared_ptr<omnisphere::repositories::TableRepository> tableRepository;
  explicit Impl(std::shared_ptr<omnisphere::services::Database> database)
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
    throw std::runtime_error(std::string("[ModifyArea Exception] ") +
                             e.what());
  }
}

std::vector<omnisphere::models::Area> Area::GetAll() const {
  try {
    std::vector<omnisphere::models::Area> areas;
    omnisphere::types::DataTable data = pImpl->areaRepository->ReadAll();

    for (int i = 0; i < data.RowsCount(); i++) {
      areas.emplace_back(data[i]["Entry"], 
                          data[i]["Code"], 
                          data[i]["Name"],
                          data[i]["Color"],
                          data[i]["Icon"],
                          data[i]["Capacity"],
                          data[i]["FloorEntry"],
                          data[i]["CreatedBy"], 
                          data[i]["CreateDate"],
                          data[i]["LastUpdatedBy"].GetOptional<int>(), 
                          data[i]["UpdateDate"].GetOptional<std::string>());
    }

    return areas;
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[GetAllAreas Exception] ") +
                             e.what());
  }
}
omnisphere::models::Area
Area::Get(const omnisphere::dtos::GetArea &getArea) const 
{
  try 
  {
    omnisphere::types::DataTable data = pImpl->areaRepository->Read(getArea);

    if (data.RowsCount() == 0)
      throw std::runtime_error("Area doesn't exists");

    return omnisphere::models::Area(data[0]["Entry"], 
                                    data[0]["Code"], 
                                    data[0]["Name"],
                                    data[0]["Color"],
                                    data[0]["Icon"],
                                    data[0]["Capacity"],
                                    data[0]["FloorEntry"],
                                    data[0]["CreatedBy"], 
                                    data[0]["CreateDate"],
                                    data[0]["LastUpdatedBy"].GetOptional<int>(), 
                                    data[0]["UpdateDate"].GetOptional<std::string>());
  } 
  catch (const std::exception &e) 
  {
    throw std::runtime_error(std::string("[GetArea Exception] ") + e.what());
  }
}
std::vector<omnisphere::models::Area> Area::Search(const omnisphere::dtos::GetArea &getArea) const {
  try {
    std::vector<omnisphere::models::Area> areas;
    omnisphere::types::DataTable data = pImpl->areaRepository->Read(getArea);

    for (int i = 0; i < data.RowsCount(); i++) {
      areas.emplace_back(data[i]["Entry"], 
                          data[i]["Code"], 
                          data[i]["Name"],
                          data[i]["Color"],
                          data[i]["Icon"],
                          data[i]["Capacity"],
                          data[i]["FloorEntry"],
                          data[i]["CreatedBy"], 
                          data[i]["CreateDate"],
                          data[i]["LastUpdatedBy"].GetOptional<int>(), 
                          data[i]["UpdateDate"].GetOptional<std::string>());
    }

    return areas;
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
    
    omnisphere::types::DataTable tables = pImpl->tableRepository->Read(getTable);
    if (tables.RowsCount() > 0) {
      throw std::runtime_error("Cannot delete Area because it has associated Tables");
    }

    return pImpl->areaRepository->Delete(entry);
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[RemoveArea Exception] ") + e.what());
  }
}
} // namespace omnisphere::area
