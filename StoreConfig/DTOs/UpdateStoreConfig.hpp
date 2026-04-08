#pragma once

#include "Base/BaseModel.hpp"
#include <optional>
#include <string>

namespace omnisphere::dtos {
    class UpdateStoreConfig : public omnisphere::models::BaseModel {
    public:
        UpdateStoreConfig(int _Entry, 
            std::string _Code, 
            std::string _Name, 
            std::optional<std::string> _Address,
            int _CreatedBy,
            std::string _CreateDate, 
            std::optional<int> _LastUpdatedBy,
            std::optional<std::string> _UpdateDate)
            : omnisphere::models::BaseModel(_Entry, 
                std::move(_Code), 
                std::move(_Name),
                _CreatedBy, 
                std::move(_CreateDate),
                _LastUpdatedBy, 
                std::move(_UpdateDate)),
              Address(std::move(_Address)) {}

        const std::optional<std::string> Address;
    };
}
