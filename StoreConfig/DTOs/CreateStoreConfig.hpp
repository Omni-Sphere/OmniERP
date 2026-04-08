#pragma once

#include "Base/BaseModel.hpp"
#include <optional>
#include <string>

namespace omnisphere::dtos {
    class CreateStoreConfig : public omnisphere::models::BaseModel {
    public:
        CreateStoreConfig(int _Entry, 
            std::string _Code, 
            std::string _Name, 
            std::optional<std::string> _Address,
            int _CreatedBy,
            std::string _CreateDate)
            : omnisphere::models::BaseModel(_Entry, 
                std::move(_Code), 
                std::move(_Name),
                _CreatedBy, 
                std::move(_CreateDate),
                std::nullopt, 
                std::nullopt),
              Address(std::move(_Address)) {}

        const std::optional<std::string> Address;
    };
}
