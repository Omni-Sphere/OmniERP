#include <Database.hpp>
#include <DataTable.hpp>
#include <Store/Repositories/Store.hpp>
#include <SQLParams.hpp>

namespace omnisphere::repositories
{
    Store::Store(std::shared_ptr<omnisphere::services::Database> database) : Database(database) {}

    Store::~Store() = default;

    template <typename T>
    void Store::AddInsertParam(const std::string &field, const T &value,
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
    void Store::AddSetParam(const std::string &field, const T &value,
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

    bool Store::Create(const omnisphere::dtos::CreateStore &_store) const
    {
        try
        {
            std::vector<std::string> fields;
            std::vector<omnisphere::types::SQLParam> params;

            AddInsertParam("Entry", GetCurrentSequence(), fields, params);
            AddInsertParam("Code", _store.Code, fields, params);
            AddInsertParam("Name", _store.Name, fields, params);
            AddInsertParam("GuestCustomer", _store.GuestCustomer, fields, params);
            AddInsertParam("Address", _store.Address, fields, params);
            AddInsertParam("Address2", _store.Address2, fields, params);
            AddInsertParam("City", _store.City, fields, params);
            AddInsertParam("State", _store.State, fields, params);
            AddInsertParam("ZipCode", _store.ZipCode, fields, params);
            AddInsertParam("Country", _store.Country, fields, params);
            AddInsertParam("TaxID", _store.TaxID, fields, params);
            AddInsertParam("Currency", _store.Currency, fields, params);
            AddInsertParam("Phone1", _store.Phone1, fields, params);
            AddInsertParam("Phone2", _store.Phone2, fields, params);
            AddInsertParam("Email", _store.Email, fields, params);
            AddInsertParam("WebSite", _store.WebSite, fields, params);
            AddInsertParam("FacebookProfile", _store.FacebookProfile, fields, params);
            AddInsertParam("InstagramProfile", _store.InstagramProfile, fields, params);
            AddInsertParam("XProfile", _store.XProfile, fields, params);
            AddInsertParam("LogoFile", _store.LogoFile, fields, params);
            AddInsertParam("IsActive", true, fields, params);
            AddInsertParam("CreatedBy", _store.CreatedBy, fields, params);
            AddInsertParam("CreateDate", _store.CreateDate, fields, params);

            std::string query = "INSERT INTO Stores (";
            std::string values = " VALUES (";

            for (size_t i = 0; i < fields.size(); ++i)
            {
                query += fields[i];
                values += "?";

                if (i < fields.size() - 1)
                {
                    query += ", ";
                    values += ", ";
                }
            }
            query += ")" + values + ")";

            if (!Database->RunPrepared(query, params))
                throw std::runtime_error("[RunPrepared exception]");

            if (!UpdateStoreSequence())
                throw std::runtime_error("[UpdateStoreSequence exception]");

            Database->CommitTransaction();

            return true;
        }
        catch (const std::exception &e)
        {
            Database->RollbackTransaction();
            throw std::runtime_error(std::string("[CreateStore Exception] ") + e.what());
        }
    }

    bool Store::Update(const omnisphere::dtos::UpdateStore &_store) const
    {
        try
        {
            std::string sQuery = "UPDATE Stores SET ";
            std::vector<omnisphere::types::SQLParam> params;
            std::vector<std::string> setClauses;

            AddSetParam("Code", _store.Code, setClauses, params);
            AddSetParam("Name", _store.Name, setClauses, params);
            AddSetParam("GuestCustomer", _store.GuestCustomer, setClauses, params);
            AddSetParam("Address", _store.Address, setClauses, params);
            AddSetParam("Address2", _store.Address2, setClauses, params);
            AddSetParam("City", _store.City, setClauses, params);
            AddSetParam("State", _store.State, setClauses, params);
            AddSetParam("ZipCode", _store.ZipCode, setClauses, params);
            AddSetParam("Country", _store.Country, setClauses, params);
            AddSetParam("TaxID", _store.TaxID, setClauses, params);
            AddSetParam("Currency", _store.Currency, setClauses, params);
            AddSetParam("Phone1", _store.Phone1, setClauses, params);
            AddSetParam("Phone2", _store.Phone2, setClauses, params);
            AddSetParam("Email", _store.Email, setClauses, params);
            AddSetParam("WebSite", _store.WebSite, setClauses, params);
            AddSetParam("FacebookProfile", _store.FacebookProfile, setClauses, params);
            AddSetParam("InstagramProfile", _store.InstagramProfile, setClauses, params);
            AddSetParam("XProfile", _store.XProfile, setClauses, params);
            AddSetParam("LogoFile", _store.LogoFile, setClauses, params);
            AddSetParam("IsActive", _store.IsActive, setClauses, params);
            AddSetParam("LastUpdatedBy", _store.LastUpdatedBy, setClauses, params);
            AddSetParam("UpdateDate", _store.UpdateDate, setClauses, params);

            if (setClauses.empty()) return true;

            for (size_t i = 0; i < setClauses.size(); ++i)
            {
                sQuery += setClauses[i];

                if (i < setClauses.size() - 1)
                    sQuery += ", ";
            }

            sQuery += " WHERE Entry = ?";
            params.push_back(omnisphere::types::MakeSQLParam(_store.Entry));

            if (!Database->RunPrepared(sQuery, params))
                throw std::runtime_error("[RunPrepared exception]");

            Database->CommitTransaction();

            return true;
        }
        catch (const std::exception &e)
        {
            Database->RollbackTransaction();
            throw std::runtime_error(std::string("[UpdateStore Exception] ") + e.what());
        }
    }

    omnisphere::types::DataTable Store::Read(int entry) const
    {
        try
        {
            std::string sQuery = "SELECT * FROM Stores WHERE Entry = ?";
            std::vector<omnisphere::types::SQLParam> params;
            params.push_back(omnisphere::types::MakeSQLParam(entry));

            return Database->FetchPrepared(sQuery, params);
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[ReadStore Exception] ") + e.what());
        }
    }

    omnisphere::types::DataTable Store::ReadAll() const
    {
        try
        {
            const std::string query = "SELECT * FROM Stores WHERE IsActive = 'Y'";

            return Database->FetchResults(query);
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[ReadAllStore Exception] ") + e.what());
        }
    }

    int Store::GetCurrentSequence() const
    {
        try
        {
            const std::string query = "SELECT ISNULL(StoreSequence, 0) + 1 StoreSequence FROM Sequences WHERE Entry = 1";
            omnisphere::types::DataTable dataTable = Database->FetchResults(query);

            return dataTable[0]["StoreSequence"];
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[GetCurrentSequence Exception] ") + e.what());
        }
    }

    bool Store::UpdateStoreSequence() const
    {
        try
        {
            const std::string query = "UPDATE Sequences SET StoreSequence = ISNULL(StoreSequence, 0) + 1";

            if (!Database->RunStatement(query))
                throw std::runtime_error("[RunStatement exception]");

            return true;
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[UpdateStoreSequence Exception] ") + e.what());
        }
    }
}
