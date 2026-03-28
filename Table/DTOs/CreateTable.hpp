#pragma once
#include "Base/BaseCreateDTO.hpp"
#include <string>

namespace omnisphere::dtos {
    struct CreateTable : public omnisphere::dtos::BaseCreateDTO {
        using omnisphere::dtos::BaseCreateDTO::BaseCreateDTO;
        int Capacity;
        std::string Type;
        int AreaEntry;
        int FloorEntry;
    };
}
