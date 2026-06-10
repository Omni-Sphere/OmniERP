#include <Company/Repositories/Company.hpp>
#include <DataTable.hpp>
#include <Database.hpp>
#include <SQLParams.hpp>

namespace omnisphere::repositories
{
    Company::Company(
        std::shared_ptr<omnisphere::services::Database> database)
        : Database(database) {}

    Company::~Company() = default;

    template <typename T>
    void Company::AddInsertParam(
        const std::string &field, const T &value,
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
    void Company::AddSetParam(
        const std::string &field, const T &value,
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

    bool Company::Create(
        const omnisphere::dtos::CreateCompany &_company) const
    {
        try
        {
            std::string sQuery = "INSERT INTO Company (";
            std::vector<omnisphere::types::SQLParam> params;
            std::vector<std::string> insertClauses;

            AddInsertParam("Entry", _company.Entry, insertClauses, params);
            AddInsertParam("Code", _company.Code, insertClauses, params);
            AddInsertParam("Name", _company.Name, insertClauses, params);
            AddInsertParam("CommercialName", _company.CommercialName, insertClauses, params);
            AddInsertParam("Address", _company.Address, insertClauses, params);
            AddInsertParam("Address2", _company.Address2, insertClauses, params);
            AddInsertParam("City", _company.City, insertClauses, params);
            AddInsertParam("State", _company.State, insertClauses, params);
            AddInsertParam("ZipCode", _company.ZipCode, insertClauses, params);
            AddInsertParam("Country", _company.Country, insertClauses, params);
            AddInsertParam("TaxID", _company.TaxID, insertClauses, params);
            AddInsertParam("Currency", _company.Currency, insertClauses, params);
            AddInsertParam("Phone1", _company.Phone1, insertClauses, params);
            AddInsertParam("Phone2", _company.Phone2, insertClauses, params);
            AddInsertParam("Email", _company.Email, insertClauses, params);
            AddInsertParam("WebSite", _company.WebSite, insertClauses, params);
            AddInsertParam("FacebookProfile", _company.FacebookProfile,
                           insertClauses, params);
            AddInsertParam("InstagramProfile", _company.InstagramProfile,
                           insertClauses, params);
            AddInsertParam("XProfile", _company.XProfile, insertClauses, params);
            AddInsertParam("LogoFile", _company.LogoFile, insertClauses, params);

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

            if (sQuery.find("IsActive") == std::string::npos)
            {
                size_t insertCloseParen = sQuery.find(")");

                if (insertCloseParen != std::string::npos)
                {
                    sQuery.insert(insertCloseParen, ", IsActive");
                }
                size_t valuesCloseParen = sQuery.find(")", insertCloseParen + 1);

                if (valuesCloseParen != std::string::npos)
                {
                    sQuery.insert(valuesCloseParen, ", 'Y'");
                }
            }

            if (!Database->RunPrepared(sQuery, params, "Company::Create"))
                throw std::runtime_error("[RunPrepared exception]");

            Database->CommitTransaction();

            return true;
        }
        catch (const std::exception &e)
        {
            Database->RollbackTransaction();
            throw std::runtime_error(std::string("[CreateCompany Exception] ") +
                                     e.what());
        }
    }

    bool Company::Update(
        const omnisphere::dtos::UpdateCompany &_company) const
    {
        try
        {
            std::string sQuery = "UPDATE Company SET ";
            std::vector<omnisphere::types::SQLParam> params;
            std::vector<std::string> setClauses;

            AddSetParam("Code", _company.Code, setClauses, params);
            AddSetParam("Name", _company.Name, setClauses, params);
            AddSetParam("CommercialName", _company.CommercialName, setClauses, params);
            AddSetParam("Address", _company.Address, setClauses, params);
            AddSetParam("Address2", _company.Address2, setClauses, params);
            AddSetParam("City", _company.City, setClauses, params);
            AddSetParam("State", _company.State, setClauses, params);
            AddSetParam("ZipCode", _company.ZipCode, setClauses, params);
            AddSetParam("Country", _company.Country, setClauses, params);
            AddSetParam("TaxID", _company.TaxID, setClauses, params);
            AddSetParam("Currency", _company.Currency, setClauses, params);
            AddSetParam("Phone1", _company.Phone1, setClauses, params);
            AddSetParam("Phone2", _company.Phone2, setClauses, params);
            AddSetParam("Email", _company.Email, setClauses, params);
            AddSetParam("WebSite", _company.WebSite, setClauses, params);
            AddSetParam("FacebookProfile", _company.FacebookProfile, setClauses,
                        params);
            AddSetParam("InstagramProfile", _company.InstagramProfile,
                        setClauses, params);
            AddSetParam("XProfile", _company.XProfile, setClauses, params);
            AddSetParam("LogoFile", _company.LogoFile, setClauses, params);
            AddSetParam("IsActive", _company.IsActive, setClauses,
                        params);

            for (size_t i = 0; i < setClauses.size(); ++i)
            {
                sQuery += setClauses[i];

                if (i < setClauses.size() - 1)
                    sQuery += ", ";
            }

            sQuery += " WHERE Entry = ?";
            params.push_back(omnisphere::types::MakeSQLParam(_company.Entry));

            if (!Database->RunPrepared(sQuery, params, "Company::Update"))
                throw std::runtime_error("[RunPrepared exception]");

            Database->CommitTransaction();

            return true;
        }
        catch (const std::exception &e)
        {
            Database->RollbackTransaction();
            throw std::runtime_error(std::string("[UpdateCompany Exception] ") +
                                     e.what());
        }
    }

    omnisphere::types::DataTable Company::Read(
        const omnisphere::dtos::GetCompany &getCompany) const
    {
        try
        {
            std::string sQuery = "SELECT [Entry], [Code], [Name], CommercialName, Address, "
            "Address2, City, State, ZipCode, Country, TaxID, Currency, Phone1, Phone2, Email, WebSite, "
            "FacebookProfile, InstagramProfile, XProfile, LogoFile, [IsActive] "
            "FROM Company ";

            return Database->FetchResults(sQuery, "Company::Read");
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[ReadCompany Exception] ") +
                                     e.what());
        }
    }
} // namespace omnisphere::repositories