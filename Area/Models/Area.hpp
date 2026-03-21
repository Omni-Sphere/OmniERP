#pragma once
#include <string>
#include <optional>
#include "Base/BaseModel.hpp"

namespace omnisphere::models {
    class Area : public omnisphere::models::BaseModel {
    public:
        Area(int _Entry, std::string _Code, std::string _Name, std::string _Color,
             std::string _Icon, int _Capacity, int _FloorEntry, int _CreatedBy,
             std::string _CreateDate, std::optional<int> _LastUpdatedBy,
             std::optional<std::string> _UpdateDate)
            : omnisphere::models::BaseModel(_Entry, std::move(_Code), std::move(_Name),
                                            _CreatedBy, std::move(_CreateDate),
                                            _LastUpdatedBy, std::move(_UpdateDate)),
              Color(std::move(_Color)), Icon(std::move(_Icon)), Capacity(_Capacity), FloorEntry(_FloorEntry) {}

        std::string Color;        
        int Capacity;
        int FloorEntry;
        std::string Icon;
    };
}