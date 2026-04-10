#include "BusinessConfig.hpp"
#include "SQLParams.hpp"

namespace omnisphere::repositories 
{
    BusinessConfig::BusinessConfig(std::shared_ptr<omnisphere::services::Database> database) : Database(database)
    {}

    BusinessConfig::~BusinessConfig() = default;

    template <typename T>
    void BusinessConfig::AddInsertParam(const std::string &field, const T &value, 
                                        std::vector<std::string> &insertClauses, 
                                        std::vector<omnisphere::types::SQLParam> &params) const
    {
        using U = std::decay_t<T>;

        if constexpr (omnisphere::types::is_optional<U>::value)
        {
            if (value.has_value())
            {
                insertClauses.push_back(field);
                params.push_back(omnisphere::types::MakeSQLParam(value.value()));
            }
        }
        else
        {
            insertClauses.push_back(field);
            params.push_back(omnisphere::types::MakeSQLParam(value));
        }
    }

    template <typename T>
    void BusinessConfig::AddSetParam(const std::string &field, const T &value, 
                                    std::vector<std::string> &setClauses, 
                                    std::vector<omnisphere::types::SQLParam> &params) const
    {
        using U = std::decay_t<T>;

        if constexpr (omnisphere::types::is_optional<U>::value)
        {
            if (value.has_value())
            {
                setClauses.push_back(field + " = ?");
                params.push_back(omnisphere::types::MakeSQLParam(value.value()));
            }
        }
        else
        {
            setClauses.push_back(field + " = ?");
            params.push_back(omnisphere::types::MakeSQLParam(value));
        }
    }

    bool BusinessConfig::Create(const omnisphere::dtos::CreateBusinessConfig &_businessConfig) const 
    {
        try 
        {
            std::string sQuery = "INSERT INTO BusinessConfig (";
            std::vector<omnisphere::types::SQLParam> params;
            std::vector<std::string> insertClauses;

            AddInsertParam("Entry", _businessConfig.Entry, insertClauses, params);
            AddInsertParam("Code", _businessConfig.Code, insertClauses, params);
            AddInsertParam("Name", _businessConfig.Name, insertClauses, params);
            AddInsertParam("Address", _businessConfig.Address, insertClauses, params);
            AddInsertParam("Address2", _businessConfig.Address2, insertClauses, params);
            AddInsertParam("City", _businessConfig.City, insertClauses, params);
            AddInsertParam("State", _businessConfig.State, insertClauses, params);
            AddInsertParam("ZipCode", _businessConfig.ZipCode, insertClauses, params);
            AddInsertParam("Country", _businessConfig.Country, insertClauses, params);
            AddInsertParam("TaxID", _businessConfig.TaxID, insertClauses, params);
            AddInsertParam("Currency", _businessConfig.Currency, insertClauses, params);
            AddInsertParam("Phone1", _businessConfig.Phone1, insertClauses, params);
            AddInsertParam("Phone2", _businessConfig.Phone2, insertClauses, params);
            AddInsertParam("Email", _businessConfig.Email, insertClauses, params);
            AddInsertParam("WebSite", _businessConfig.WebSite, insertClauses, params);
            AddInsertParam("FacebookProfile", _businessConfig.FacebookProfile, insertClauses, params);
            AddInsertParam("InstagramProfile", _businessConfig.InstagramProfile, insertClauses, params);
            AddInsertParam("XProfile", _businessConfig.XProfile, insertClauses, params);
            AddInsertParam("LogoFile", _businessConfig.LogoFile, insertClauses, params);
            AddInsertParam("AttachmentsPath", _businessConfig.AttachmentsPath, insertClauses, params);

            for (size_t i = 0; i < insertClauses.size(); ++i) 
            {
                sQuery += insertClauses[i];
                if (i < insertClauses.size() - 1)
                    sQuery += ", ";
            }

            sQuery += ") VALUES (";

            for (size_t i = 0; i < params.size(); ++i) 
            {
                sQuery += "?";
                if (i < params.size() - 1)
                    sQuery += ", ";
            }

            sQuery += ")";

            // Add IsActive logic for "all tables" rule
            if (sQuery.find("IsActive") == std::string::npos) {
                size_t insertCloseParen = sQuery.find(")");
                if (insertCloseParen != std::string::npos) {
                    sQuery.insert(insertCloseParen, ", IsActive");
                }
                size_t valuesCloseParen = sQuery.find(")", insertCloseParen + 1);
                if (valuesCloseParen != std::string::npos) {
                  sQuery.insert(valuesCloseParen, ", 'Y'");
                }
            }

            if (!Database->RunPrepared(sQuery, params))
                throw std::runtime_error("[RunPrepared exception]");

            Database->CommitTransaction();

            return true;
        } 
        catch (const std::exception &e) 
        {
            Database->RollbackTransaction();
            throw std::runtime_error(std::string("[CreateBusinessConfig Exception] ") + e.what());
        }
    }

    bool BusinessConfig::Update(const omnisphere::dtos::UpdateBusinessConfig &_businessConfig) const 
    {
        try 
        {
            std::string sQuery = "UPDATE BusinessConfig SET ";
            std::vector<omnisphere::types::SQLParam> params;
            std::vector<std::string> setClauses;

            AddSetParam("Code", _businessConfig.Code, setClauses, params);
            AddSetParam("Name", _businessConfig.Name, setClauses, params);
            AddSetParam("Address", _businessConfig.Address, setClauses, params);
            AddSetParam("Address2", _businessConfig.Address2, setClauses, params);
            AddSetParam("City", _businessConfig.City, setClauses, params);
            AddSetParam("State", _businessConfig.State, setClauses, params);
            AddSetParam("ZipCode", _businessConfig.ZipCode, setClauses, params);
            AddSetParam("Country", _businessConfig.Country, setClauses, params);
            AddSetParam("TaxID", _businessConfig.TaxID, setClauses, params);
            AddSetParam("Currency", _businessConfig.Currency, setClauses, params);
            AddSetParam("Phone1", _businessConfig.Phone1, setClauses, params);
            AddSetParam("Phone2", _businessConfig.Phone2, setClauses, params);
            AddSetParam("Email", _businessConfig.Email, setClauses, params);
            AddSetParam("WebSite", _businessConfig.WebSite, setClauses, params);
            AddSetParam("FacebookProfile", _businessConfig.FacebookProfile, setClauses, params);
            AddSetParam("InstagramProfile", _businessConfig.InstagramProfile, setClauses, params);
            AddSetParam("XProfile", _businessConfig.XProfile, setClauses, params);
            AddSetParam("LogoFile", _businessConfig.LogoFile, setClauses, params);
            AddSetParam("AttachmentsPath", _businessConfig.AttachmentsPath, setClauses, params);

            for (size_t i = 0; i < setClauses.size(); ++i) 
            {
                sQuery += setClauses[i];
                if (i < setClauses.size() - 1)
                    sQuery += ", ";
            }

            sQuery += " WHERE Entry = ?";
            params.push_back(omnisphere::types::MakeSQLParam(_businessConfig.Entry));

            if (!Database->RunPrepared(sQuery, params))
                throw std::runtime_error("[RunPrepared exception]");

            Database->CommitTransaction();

            return true;
        } 
        catch (const std::exception &e) 
        {
            Database->RollbackTransaction();
            throw std::runtime_error(std::string("[UpdateBusinessConfig Exception] ") + e.what());
        }
    }

    omnisphere::types::DataTable BusinessConfig::Read(const omnisphere::dtos::GetBusinessConfig &getBusinessConfig) const 
    {
        try 
        {
            std::string sQuery = "SELECT * FROM BusinessConfig WHERE 1 = 1 AND IsActive = 'Y'";
            std::vector<omnisphere::types::SQLParam> params;

            if (getBusinessConfig.Entry > 0)
            {
                sQuery += " AND Entry = ?";
                params.push_back(omnisphere::types::MakeSQLParam(getBusinessConfig.Entry));
            }

            if (!getBusinessConfig.Code.empty())
            {
                sQuery += " AND Code = ?";
                params.push_back(omnisphere::types::MakeSQLParam(getBusinessConfig.Code));
            }

            if (!getBusinessConfig.Name.empty())
            {
                sQuery += " AND Name = ?";
                params.push_back(omnisphere::types::MakeSQLParam(getBusinessConfig.Name));
            }

            return Database->FetchPrepared(sQuery, params);
        } 
        catch (const std::exception &e) 
        {
            throw std::runtime_error(std::string("[ReadBusinessConfig Exception] ") + e.what());
        }
    }
}