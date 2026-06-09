#include <Database.hpp>
#include <DataTable.hpp>
#pragma once
#include <string>
#include <optional>

namespace omnisphere::dtos
{
    struct CreateEmployee
    {
        std::string Code;
        std::string Name;
        std::string FirstName;
        std::optional<std::string> SecondName;
        std::string LastName;
        std::optional<std::string> SecondLastName;
        std::optional<std::string> Phone;
        std::optional<std::string> DateOfBirth;
        std::optional<std::string> PlaceOfBirth;
        std::optional<std::string> Comments;
        int CreatedBy;
    };

    struct UpdateEmployee
    {
        int Entry;
        std::optional<std::string> Code;
        std::optional<std::string> Name;
        std::optional<std::string> FirstName;
        std::optional<std::string> SecondName;
        std::optional<std::string> LastName;
        std::optional<std::string> SecondLastName;
        std::optional<std::string> Phone;
        std::optional<std::string> DateOfBirth;
        std::optional<std::string> PlaceOfBirth;
        std::optional<std::string> Comments;
        std::optional<bool> IsActive;
        int UpdatedBy;
    };

    struct GetEmployee
    {
        std::optional<int> Entry;
        std::optional<std::string> Code;
        std::optional<std::string> Name;
    };
} // namespace omnisphere::dtos
