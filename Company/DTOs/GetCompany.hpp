#include <Database.hpp>
#include <DataTable.hpp>
#pragma once

#include <string>

namespace omnisphere::dtos {
    struct GetCompany {
        int Entry = 0;
        std::string Code = "";
        std::string Name = "";
    };
}
