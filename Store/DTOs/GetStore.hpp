#include <Database.hpp>
#include <DataTable.hpp>
#pragma once

#include <string>
#include <optional>

namespace omnisphere::dtos {
    struct GetStore {
        int Entry;
        std::string Code;
        std::string Name;
        int GuestCustomer;
        std::optional<std::string> Address;
        std::optional<std::string> Address2;
        std::optional<int> City;
        std::optional<int> State;
        std::optional<int> ZipCode;
        std::optional<int> Country;
        int Currency;
        std::optional<std::string> Phone1;
        std::optional<std::string> Phone2;
        std::optional<std::string> Email;
        std::string IsActive;
    };
}
