#pragma once

#include "Base/BaseModel.hpp"
#include <string>
#include <optional>

namespace omnisphere::models {
    class BusinessConfig : public omnisphere::models::BaseModel {
    public:
        BusinessConfig(int _Entry, 
            std::string _Code, 
            std::string _Name, 
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
            std::optional<std::string> _AttachmentsPath,
            int _CreatedBy,
            std::string _CreateDate, 
            std::optional<int> _LastUpdatedBy,
            std::optional<std::string> _UpdateDate)
            : omnisphere::models::BaseModel(_Entry, 
                std::move(_Code), 
                std::move(_Name),
                _CreatedBy, 
                std::move(_CreateDate),
                _LastUpdatedBy, 
                std::move(_UpdateDate)),
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
              AttachmentsPath(std::move(_AttachmentsPath)) {}

        const std::optional<std::string> Address;
        const std::optional<std::string> Address2;
        const std::optional<int> City;
        const std::optional<int> State;
        const std::optional<int> ZipCode;
        const std::optional<int> Country;
        const std::optional<std::string> TaxID;
        const int Currency;
        const std::optional<std::string> Phone1;
        const std::optional<std::string> Phone2;
        const std::optional<std::string> Email;
        const std::optional<std::string> WebSite;
        const std::optional<std::string> FacebookProfile;
        const std::optional<std::string> InstagramProfile;
        const std::optional<std::string> XProfile;
        const std::optional<std::string> LogoFile;
        const std::optional<std::string> AttachmentsPath;
    };
}
