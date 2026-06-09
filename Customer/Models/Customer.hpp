#include <Database.hpp>
#include <DataTable.hpp>
#pragma once

#include <Base/BaseModel.hpp>
#include <string>
#include <optional>

namespace omnisphere::models
{
    class Customer : public omnisphere::models::BaseModel
    {
        public:
        Customer(
            int _Entry,
            std::string _Code,
            std::string _Name,
            std::string _FirstName,
            std::optional<std::string> _MiddleName,
            std::string _LastName,
            std::optional<std::string> _SecondLastName,
            std::optional<std::string> _TaxID,
            std::optional<std::string> _Email,
            std::optional<std::string> _Phone,
            int _PaymentTerms,
            double _MaxDiscount,
            std::optional<double> _CreditLimit,
            std::string _IsActive,
            int _CreatedBy,
            std::string _CreateDate,
            std::optional<int> _LastUpdatedBy,
            std::optional<std::string> _UpdateDate
        )
            : omnisphere::models::BaseModel(
                _Entry,
                std::move(_Code),
                std::move(_Name),
                _CreatedBy,
                std::move(_CreateDate),
                _LastUpdatedBy,
                std::move(_UpdateDate)
            ),
            FirstName(std::move(_FirstName)),
            MiddleName(std::move(_MiddleName)),
            LastName(std::move(_LastName)),
            SecondLastName(std::move(_SecondLastName)),
            TaxID(std::move(_TaxID)),
            Email(std::move(_Email)),
            Phone(std::move(_Phone)),
            PaymentTerms(_PaymentTerms),
            MaxDiscount(_MaxDiscount),
            CreditLimit(_CreditLimit),
            IsActive(std::move(_IsActive)) {}

        const std::string FirstName;
        const std::optional<std::string> MiddleName;
        const std::string LastName;
        const std::optional<std::string> SecondLastName;
        const std::optional<std::string> TaxID;
        const std::optional<std::string> Email;
        const std::optional<std::string> Phone;
        const int PaymentTerms;
        const double MaxDiscount;
        const std::optional<double> CreditLimit;
        const std::string IsActive;
    };

}
