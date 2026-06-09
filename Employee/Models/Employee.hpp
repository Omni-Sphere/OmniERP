#include <Database.hpp>
#include <DataTable.hpp>
#pragma once
#include <string>
#include <optional>
#include <Base/BaseModel.hpp>

namespace omnisphere::models
{
    class Employee : public BaseModel
    {
        public:
        Employee(
            int _Entry,
            std::string _Code,
            std::string _Name,
            std::string _FirstName,
            std::optional<std::string> _SecondName,
            std::string _LastName,
            std::optional<std::string> _SecondLastName,
            std::optional<std::string> _Phone,
            std::optional<std::string> _DateOfBirth,
            std::optional<std::string> _PlaceOfBirth,
            std::optional<std::string> _Comments,
            bool _IsActive,
            int _CreatedBy,
            std::string _CreateDate,
            std::optional<int> _LastUpdatedBy,
            std::optional<std::string> _UpdateDate
        )
            : BaseModel(
                _Entry,
                std::move(_Code),
                std::move(_Name),
                _CreatedBy,
                std::move(_CreateDate),
                _LastUpdatedBy,
                std::move(_UpdateDate)
            ),
            FirstName(std::move(_FirstName)),
            SecondName(std::move(_SecondName)),
            LastName(std::move(_LastName)),
            SecondLastName(std::move(_SecondLastName)),
            Phone(std::move(_Phone)),
            DateOfBirth(std::move(_DateOfBirth)),
            PlaceOfBirth(std::move(_PlaceOfBirth)),
            Comments(std::move(_Comments)),
            IsActive(_IsActive) {}

        std::string FirstName;
        std::optional<std::string> SecondName;
        std::string LastName;
        std::optional<std::string> SecondLastName;
        std::optional<std::string> Phone;
        std::optional<std::string> DateOfBirth;
        std::optional<std::string> PlaceOfBirth;
        std::optional<std::string> Comments;
        bool IsActive;
    };
} // namespace omnisphere::models
