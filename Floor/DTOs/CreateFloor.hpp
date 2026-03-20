#pragma once
#include "Base/BaseCreateDTO.hpp"
#include "Base/BaseUpdateDTO.hpp"
#include <string>
#include <optional>

namespace omnisphere::dtos {
    struct CreateFloor : public omnisphere::dtos::BaseCreateDTO {
        using omnisphere::dtos::BaseCreateDTO::BaseCreateDTO;
    };
}
