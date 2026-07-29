#include <Database.hpp>
#include <DataTable.hpp>
#pragma once

#include <Base/BaseModel.hpp>
#include <string>
#include <optional>
#include <boost/describe.hpp>

namespace omnisphere::models
{
    class Store : public omnisphere::models::BaseModel
    {
        public:
        Store() = default;

        Store(
            int _Entry,
            std::string _Code,
            std::string _Name,
            int _GuestCustomer,
            std::optional<std::string> _Address,
            std::optional<std::string> _Address2,
            std::optional<int> _City,
            std::optional<int> _State,
            std::optional<int> _ZipCode,
            std::optional<int> _Country,
            std::optional<std::string> _TaxID,
            int _Currency,
            std::optional<std::string> _Phone1,
            std::optional<std::string> _Phone2,
            std::optional<std::string> _Email,
            std::optional<std::string> _WebSite,
            std::optional<std::string> _FacebookProfile,
            std::optional<std::string> _InstagramProfile,
            std::optional<std::string> _XProfile,
            std::optional<std::string> _LogoFile,
            std::optional<std::string> _ImagePath,
            std::optional<std::string> _ReportsPath,
            std::optional<std::string> _TicketsPath,
            bool _IsActive,
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
            GuestCustomer(_GuestCustomer),
            Address(std::move(_Address)),
            Address2(std::move(_Address2)),
            City(_City),
            State(_State),
            ZipCode(_ZipCode),
            Country(_Country),
            TaxID(std::move(_TaxID)),
            Currency(_Currency),
            Phone1(std::move(_Phone1)),
            Phone2(std::move(_Phone2)),
            Email(std::move(_Email)),
            WebSite(std::move(_WebSite)),
            FacebookProfile(std::move(_FacebookProfile)),
            InstagramProfile(std::move(_InstagramProfile)),
            XProfile(std::move(_XProfile)),
            LogoFile(std::move(_LogoFile)),
            ImagePath(std::move(_ImagePath)),
            ReportsPath(std::move(_ReportsPath)),
            TicketsPath(std::move(_TicketsPath)),
            IsActive(_IsActive) {}

        int GuestCustomer = 0;
        std::optional<std::string> Address;
        std::optional<std::string> Address2;
        std::optional<int> City;
        std::optional<int> State;
        std::optional<int> ZipCode;
        std::optional<int> Country;
        std::optional<std::string> TaxID;
        int Currency = 1;
        std::optional<std::string> Phone1;
        std::optional<std::string> Phone2;
        std::optional<std::string> Email;
        std::optional<std::string> WebSite;
        std::optional<std::string> FacebookProfile;
        std::optional<std::string> InstagramProfile;
        std::optional<std::string> XProfile;
        std::optional<std::string> LogoFile;
        std::optional<std::string> ImagePath;
        std::optional<std::string> ReportsPath;
        std::optional<std::string> TicketsPath;
        bool IsActive = true;
    };

    BOOST_DESCRIBE_STRUCT(Store, (), (Entry, Code, Name, GuestCustomer, Address, Address2, City, State, ZipCode, Country, TaxID, Currency, Phone1, Phone2, Email, WebSite, FacebookProfile, InstagramProfile, XProfile, LogoFile, ImagePath, ReportsPath, TicketsPath, IsActive, CreatedBy, CreateDate, LastUpdatedBy, UpdateDate))
}
