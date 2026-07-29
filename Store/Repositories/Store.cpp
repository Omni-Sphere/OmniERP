#include <Database.hpp>
#include <DataTable.hpp>
#include <Store/Repositories/Store.hpp>
#include <SQLParams.hpp>
#include <QueryBuilder.hpp>

namespace omnisphere::repositories
{
    static const std::vector<std::string> storeSelectFields = {
        "Entry", "Code", "Name", "GuestCustomer", "Address", "Address2", "City", "State",
        "ZipCode", "Country", "TaxID", "Currency", "Phone1", "Phone2", "Email", "WebSite",
        "FacebookProfile", "InstagramProfile", "XProfile", "LogoFile", "ImagePath",
        "ReportsPath", "TicketsPath", "IsActive", "CreatedBy", "CreateDate", "LastUpdatedBy", "UpdateDate"
    };

    Store::Store(std::shared_ptr<omnisphere::services::Database> database) : Database(std::move(database)) {}

    Store::~Store() = default;

    bool Store::Create(const omnisphere::dtos::CreateStore &_store) const
    {
        try
        {
            Database->BeginTransaction();

            auto insertData = omnisphere::types::BuildInsertQuery("Stores", GetCurrentSequence(), _store);

            if (!Database->RunPrepared(insertData.Query, insertData.Parameters, "Store::Create"))
                throw std::runtime_error("Error creating store");

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
            auto cols = omnisphere::types::ExtractUpdateColumns(_store);

            cols.push_back({"LastUpdatedBy", omnisphere::types::MakeSQLParam(_store.LastUpdatedBy)});
            cols.push_back({"UpdateDate", omnisphere::types::MakeSQLParam(_store.UpdateDate)});

            auto updateResult = omnisphere::types::BuildUpdateQuery("Stores", cols, "Entry", omnisphere::types::MakeSQLParam(_store.Entry));

            if (!Database->RunPrepared(updateResult.Query, updateResult.Parameters, "Store::Update"))
                throw std::runtime_error("Error updating store");

            return true;
        }
        catch (const std::exception &e)
        {
            Database->RollbackTransaction();
            throw std::runtime_error(std::string("[UpdateStore Exception] ") + e.what());
        }
    }

    omnisphere::types::DataTable Store::Read(int entry, const std::vector<std::string>& fields) const
    {
        try
        {
            const std::vector<std::string>& selectFields = fields.empty() ? storeSelectFields : fields;
            std::vector<omnisphere::types::Condition> conditions = {{"", "Entry", "=", "?"}};
            std::vector<omnisphere::types::SQLParam> params = {omnisphere::types::MakeSQLParam(entry)};

            auto qp = omnisphere::types::BuildQueryParts(selectFields, conditions);
            std::string sQuery = "SELECT " + qp.SelectClause + " FROM Stores WHERE " + qp.WhereClause;

            return Database->FetchPrepared(sQuery, params, "Store::Read");
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[ReadStore Exception] ") + e.what());
        }
    }

    omnisphere::types::DataTable Store::ReadByCode(const std::string& code, const std::vector<std::string>& fields) const
    {
        try
        {
            const std::vector<std::string>& selectFields = fields.empty() ? storeSelectFields : fields;
            std::vector<omnisphere::types::Condition> conditions = {{"", "Code", "=", "?"}};
            std::vector<omnisphere::types::SQLParam> params = {omnisphere::types::MakeSQLParam(code)};

            auto qp = omnisphere::types::BuildQueryParts(selectFields, conditions);
            std::string sQuery = "SELECT " + qp.SelectClause + " FROM Stores WHERE " + qp.WhereClause;

            return Database->FetchPrepared(sQuery, params, "Store::ReadByCode");
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[ReadStoreByCode Exception] ") + e.what());
        }
    }

    omnisphere::types::DataTable Store::ReadAll(const std::vector<std::string>& fields) const
    {
        try
        {
            const std::vector<std::string>& selectFields = fields.empty() ? storeSelectFields : fields;
            auto qp = omnisphere::types::BuildQueryParts(selectFields, {});
            std::string sQuery = "SELECT " + qp.SelectClause + " FROM Stores WHERE IsActive = 'Y'";

            return Database->FetchResults(sQuery, "Store::ReadAll");
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[ReadAllStore Exception] ") + e.what());
        }
    }

    omnisphere::types::DataTable Store::Search(const std::vector<std::string>& fields, const omnisphere::dtos::GetStore &filter) const
    {
        try
        {
            std::vector<omnisphere::types::Condition> conditions;
            std::vector<omnisphere::types::SQLParam> sqlParams;

            if (filter.Entry > 0)
            {
                conditions.push_back({"", "Entry", "=", "?"});
                sqlParams.push_back(omnisphere::types::MakeSQLParam(filter.Entry));
            }
            if (!filter.Code.empty())
            {
                conditions.push_back({"", "Code", "=", "?"});
                sqlParams.push_back(omnisphere::types::MakeSQLParam(filter.Code));
            }
            if (!filter.Name.empty())
            {
                conditions.push_back({"", "Name", "LIKE", "?"});
                sqlParams.push_back(omnisphere::types::MakeSQLParam("%" + filter.Name + "%"));
            }

            const std::vector<std::string>& selectFields = fields.empty() ? storeSelectFields : fields;
            auto qp = omnisphere::types::BuildQueryParts(selectFields, conditions);

            std::string sQuery = "SELECT " + qp.SelectClause + " FROM Stores";
            if (!qp.WhereClause.empty())
            {
                sQuery += " WHERE " + qp.WhereClause;
            }

            return Database->FetchPrepared(sQuery, sqlParams, "Store::Search");
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[SearchStore Exception] ") + e.what());
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
