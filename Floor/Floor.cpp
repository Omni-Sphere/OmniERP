#include "Area/Repositories/Area.hpp"
#include "Floor/Floor.hpp"
#include "Floor/Repositories/Floor.hpp"
#include <OmniData/DataTable.hpp>
#include <OmniData/Database.hpp>
#include <OmniData/DataMapper.hpp>
#include <stdexcept>
#include <string>

namespace omnisphere::floor {
Floor::Floor(std::shared_ptr<omnisphere::data::Database> database)
    : pImpl(std::make_unique<Impl>(database)) {}
Floor::~Floor() = default;

struct Floor::Impl {
  std::shared_ptr<omnisphere::repositories::FloorRepository> floorRepository;
  std::shared_ptr<omnisphere::repositories::AreaRepository> areaRepository;
  explicit Impl(std::shared_ptr<omnisphere::data::Database> database)
      : floorRepository(
            std::make_shared<omnisphere::repositories::FloorRepository>(
                database)),
        areaRepository(
            std::make_shared<omnisphere::repositories::AreaRepository>(
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

std::vector<omnisphere::models::Floor> Floor::GetAll(const std::vector<std::string>& fields) const {
  try {
    omnisphere::types::DataTable data = pImpl->floorRepository->ReadAll(fields);
    return omnisphere::types::DataTableToModels<omnisphere::models::Floor>(data);
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[GetAllFloors Exception] ") +
                             e.what());
  }
}
omnisphere::models::Floor
Floor::Get(const omnisphere::dtos::GetFloor &getFloor, const std::vector<std::string>& fields) const {
  try {
    omnisphere::types::DataTable data = pImpl->floorRepository->Read(getFloor, fields);

    if (data.RowsCount() == 0)
      throw std::runtime_error("Floor doesn't exists");

    return omnisphere::types::FromDataRow<omnisphere::models::Floor>(data[0]);
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[GetFloor Exception] ") + e.what());
  }
}

bool Floor::Remove(int entry) const {
  try {
    // Check for associated areas
    omnisphere::dtos::GetArea getArea;
    getArea.FloorEntry = entry;

    omnisphere::types::DataTable areas = pImpl->areaRepository->Read(getArea);

    if (areas.RowsCount() > 0) {
      throw std::runtime_error(
          "Cannot delete Floor because it has associated Areas");
    }

    return pImpl->floorRepository->Delete(entry);
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[RemoveFloor Exception] ") +
                             e.what());
  }
}
} // namespace omnisphere::floor