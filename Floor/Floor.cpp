#include "Floor.hpp"
#include "DataTable.hpp"
#include "Repositories/Floor.hpp"
#include <stdexcept>
#include <string>

namespace omnisphere::floor {
Floor::Floor(std::shared_ptr<omnisphere::services::Database> database)
    : pImpl(std::make_unique<Impl>(database)) {}
Floor::~Floor() = default;

struct Floor::Impl {
  std::shared_ptr<omnisphere::repositories::FloorRepository> floorRepository;
  explicit Impl(std::shared_ptr<omnisphere::services::Database> database)
      : floorRepository(
            std::make_shared<omnisphere::repositories::FloorRepository>(
                database)) {}
};

omnisphere::models::Floor
Floor::Add(const omnisphere::dtos::CreateFloor &floor) const {
  try {
    if (pImpl->floorRepository->Create(floor)) {
      omnisphere::dtos::GetFloor getFloor;
      getFloor.Code = floor.Code;
      return Get(getFloor);
    } else {
      throw std::runtime_error("Error adding floor");
    }
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[AddFloor Exception] ") + e.what());
  }
}
omnisphere::models::Floor
Floor::Modify(const omnisphere::dtos::UpdateFloor &floor) const {
  try {
    if (pImpl->floorRepository->Update(floor)) {
      omnisphere::dtos::GetFloor getFloor;
      getFloor.Code = floor.Code;
      return Get(getFloor);
    } else {
      throw std::runtime_error("Error updating floor");
    }
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[ModifyFloor Exception] ") +
                             e.what());
  }
}

std::vector<omnisphere::models::Floor> Floor::GetAll() const {
  try {
    std::vector<omnisphere::models::Floor> floors;
    omnisphere::types::DataTable data = pImpl->floorRepository->ReadAll();

    for (int i = 0; i < data.RowsCount(); i++) {
      floors.emplace_back(data[i]["Entry"], 
                          data[i]["Code"], 
                          data[i]["Name"],
                          data[i]["CreatedBy"], 
                          data[i]["CreateDate"],
                          data[i]["LastUpdatedBy"].GetOptional<int>(), 
                          data[i]["UpdateDate"].GetOptional<std::string>());
    }

    return floors;
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[GetAllFloors Exception] ") +
                             e.what());
  }
}
omnisphere::models::Floor
Floor::Get(const omnisphere::dtos::GetFloor &getFloor) const 
{
  try 
  {
    omnisphere::types::DataTable data = pImpl->floorRepository->Read(getFloor);

    if (data.RowsCount() == 0)
      throw std::runtime_error("Floor doesn't exists");

    return omnisphere::models::Floor(data[0]["Entry"], 
                                    data[0]["Code"], 
                                    data[0]["Name"],
                                    data[0]["CreatedBy"], 
                                    data[0]["CreateDate"],
                                    data[0]["LastUpdatedBy"].GetOptional<int>(), 
                                    data[0]["UpdateDate"].GetOptional<std::string>());
  } 
  catch (const std::exception &e) 
  {
    throw std::runtime_error(std::string("[GetFloor Exception] ") + e.what());
  }
}
} // namespace omnisphere::floor