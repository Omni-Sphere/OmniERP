#pragma once
#include <OmniCore/Base/BaseModel.hpp>
#include <optional>
#include <string>

namespace omnisphere::models {
class Table : public omnisphere::models::BaseModel {
public:
  Table(int _Entry, std::string _Code, std::string _Name, int _Capacity,
        std::string _Type, int _AreaEntry, int _FloorEntry, int _CreatedBy,
        std::string _CreateDate, std::optional<int> _LastUpdatedBy,
        std::optional<std::string> _UpdateDate,
        std::string _Status = "AVAILABLE", bool _IsActive = true)
      : omnisphere::models::BaseModel(
            _Entry, std::move(_Code), std::move(_Name), _CreatedBy,
            std::move(_CreateDate), _LastUpdatedBy, std::move(_UpdateDate)),
        Capacity(_Capacity), Type(std::move(_Type)), AreaEntry(_AreaEntry),
        FloorEntry(_FloorEntry), Status(std::move(_Status)), IsActive(_IsActive) {}

  int Capacity;
  std::string Type;
  int AreaEntry;
  int FloorEntry;
  std::string Status = "AVAILABLE";
  bool IsActive = true;
};
} // namespace omnisphere::models
