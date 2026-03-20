#pragma once
#include "Base/BaseUpdateDTO.hpp"
#include <string>

namespace omnisphere::dtos 
{
    struct UpdateFloor : public omnisphere::dtos::BaseUpdateDTO {
        using omnisphere::dtos::BaseUpdateDTO::BaseUpdateDTO;     
    };
};
