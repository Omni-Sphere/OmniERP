#include <Database.hpp>
#include <DataTable.hpp>
#pragma once

#include <Base/BaseModel.hpp>
#include <optional>
#include <string>

namespace omnisphere::dtos
{
    class CreateCustomer : public omnisphere::models::BaseModel
    {
        public:
        CreateCustomer(
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
            int _CreatedBy,
            std::string _CreateDate
        )
            : omnisphere::models::BaseModel(
                _Entry,
                std::move(_Code),
                std::move(_Name),
                _CreatedBy,
                std::move(_CreateDate),
                std::nullopt,
                std::nullopt
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
            CreditLimit(_CreditLimit) {}

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
    };

}
