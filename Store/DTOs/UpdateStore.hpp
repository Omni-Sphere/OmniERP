#include <Database.hpp>
#include <DataTable.hpp>
#pragma once

#include <Base/BaseModel.hpp>
#include <optional>
#include <string>

namespace omnisphere::dtos
{
    class UpdateStore : public omnisphere::models::BaseModel
    {
        public:
        UpdateStore(int _Entry, std::string _Code, std::string _Name, int _GuestCustomer,
                    std::optional<std::string> _Address,
                    std::optional<std::string> _Address2, std::optional<int> _City,
                    std::optional<int> _State, std::optional<int> _ZipCode,
                    std::optional<int> _Country, int _Currency,
                    std::optional<std::string> _Phone1,
                    std::optional<std::string> _Phone2,
                    std::optional<std::string> _Email,
                    std::optional<std::string> _Website,
                    std::optional<std::string> _Facebook,
                    std::optional<std::string> _Instagram, std::optional<std::string> _X,
                    std::optional<std::string> _Whatsapp, std::string _IsActive,
                    int _LastUpdatedBy, std::string _UpdateDate)
            : omnisphere::models::BaseModel(_Entry, std::move(_Code), std::move(_Name),
                                            0, // CreatedBy not needed for update "",
        // CreateDate not needed for update _LastUpdatedBy,
                                            std::move(_UpdateDate)),
            GuestCustomer(_GuestCustomer), Address(std::move(_Address)),
            Address2(std::move(_Address2)), City(_City), State(_State),
            ZipCode(_ZipCode), Country(_Country), Currency(_Currency),
            Phone1(std::move(_Phone1)), Phone2(std::move(_Phone2)),
            Email(std::move(_Email)), Website(std::move(_Website)),
            Facebook(std::move(_Facebook)), Instagram(std::move(_Instagram)),
            X(std::move(_X)), Whatsapp(std::move(_Whatsapp)),
            IsActive(std::move(_IsActive)) {}

        const int GuestCustomer;
        const std::optional<std::string> Address;
        const std::optional<std::string> Address2;
        const std::optional<int> City;
        const std::optional<int> State;
        const std::optional<int> ZipCode;
        const std::optional<int> Country;
        const int Currency;
        const std::optional<std::string> Phone1;
        const std::optional<std::string> Phone2;
        const std::optional<std::string> Email;
        const std::optional<std::string> Website;
        const std::optional<std::string> Facebook;
        const std::optional<std::string> Instagram;
        const std::optional<std::string> X;
        const std::optional<std::string> Whatsapp;
        const std::string IsActive;
    };
}
