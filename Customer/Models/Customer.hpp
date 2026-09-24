#pragma once

#include <OmniCore/Base/BaseModel.hpp>
#include <boost/describe.hpp>
#include <optional>
#include <string>

namespace omnisphere::models {
class Customer : public omnisphere::models::BaseModel {
public:
  Customer() = default;

  Customer(int _Entry, std::string _Code, std::string _Name,
           std::string _FirstName, std::optional<std::string> _MiddleName,
           std::string _LastName, std::optional<std::string> _SecondLastName,
           std::optional<std::string> _TaxID, std::optional<std::string> _Email,
           std::optional<std::string> _Phone, int _PaymentTerms,
           double _MaxDiscount, std::optional<double> _CreditLimit,
           bool _IsActive, int _CreatedBy, std::string _CreateDate,
           std::optional<int> _LastUpdatedBy,
           std::optional<std::string> _UpdateDate)
      : omnisphere::models::BaseModel(
            _Entry, std::move(_Code), std::move(_Name), _CreatedBy,
            std::move(_CreateDate), _LastUpdatedBy, std::move(_UpdateDate)),
        FirstName(std::move(_FirstName)), MiddleName(std::move(_MiddleName)),
        LastName(std::move(_LastName)),
        SecondLastName(std::move(_SecondLastName)), TaxID(std::move(_TaxID)),
        Email(std::move(_Email)), Phone(std::move(_Phone)),
        PaymentTerms(_PaymentTerms), MaxDiscount(_MaxDiscount),
        CreditLimit(_CreditLimit), IsActive(_IsActive) {}

  std::string FirstName;
  std::optional<std::string> MiddleName;
  std::string LastName;
  std::optional<std::string> SecondLastName;
  std::optional<std::string> TaxID;
  std::optional<std::string> Email;
  std::optional<std::string> Phone;
  int PaymentTerms = 0;
  double MaxDiscount = 0.0;
  std::optional<double> CreditLimit;
  bool IsActive = true;
};

BOOST_DESCRIBE_STRUCT(Customer, (),
                      (Entry, Code, Name, FirstName, MiddleName, LastName,
                       SecondLastName, TaxID, Email, Phone, PaymentTerms,
                       MaxDiscount, CreditLimit, IsActive, CreatedBy,
                       CreateDate, LastUpdatedBy, UpdateDate))

} // namespace omnisphere::models
