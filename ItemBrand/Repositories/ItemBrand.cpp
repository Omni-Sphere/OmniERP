#include <Database.hpp>
#include <DataTable.hpp>
#include <QueryBuilder.hpp>
#include <ItemBrand/Repositories/ItemBrand.hpp>

namespace omnisphere::repositories
{
    static const std::vector<std::string> brandSelectFields = {
        "Entry", "Code", "Name", "CreatedBy", "CreateDate", "LastUpdatedBy", "UpdateDate"
    };

    ItemBrand::ItemBrand(std::shared_ptr<omnisphere::data::Database> _database)
        : database(std::move(_database)) {}

    int ItemBrand::GetCurrentSequence() const
    {
        try
        {
            const std::string sQuery =
            "SELECT COALESCE(ItemBrandsSequence, 0) + 1 ItemBrandsSequence FROM "
            "Sequences WHERE Entry = 1";

            omnisphere::types::DataTable data = database->FetchResults(sQuery, "ItemBrand::GetCurrentSequence");

            if (data.RowsCount() == 1)
                return data[0]["ItemBrandsSequence"];
            else
                return 0;
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[GetCurrentSequence Exception] ") +
                                     " " + e.what());
        }
    }

    bool ItemBrand::UpdateItemBrandsSequence() const
    {
        try
        {
            const std::string sQuery = "UPDATE Sequences SET ItemBrandsSequence = "
            "COALESCE(ItemBrandsSequence,0) + 1";

            if (!database->RunStatement(sQuery, "ItemBrand::UpdateItemBrandsSequence"))
                return false;

            return true;
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[ItemBrandsSequence Exception] ") +
                                     " " + e.what());
        }
    }

    bool ItemBrand::Create(
        const omnisphere::dtos::CreateItemBrand &createItemBrand) const
    {
        try
        {
            static const std::vector<std::string> insertColumns = {
                "Entry", "Code", "Name", "CreatedBy", "CreateDate"
            };
            const std::string sQuery = omnisphere::types::BuildInsertQuery("Brands", insertColumns);

            std::vector<omnisphere::types::SQLParam> params = {
                omnisphere::types::MakeSQLParam(GetCurrentSequence()),
                omnisphere::types::MakeSQLParam(createItemBrand.Code),
                omnisphere::types::MakeSQLParam(createItemBrand.Name),
                omnisphere::types::MakeSQLParam(createItemBrand.CreatedBy),
                omnisphere::types::MakeSQLParam(createItemBrand.CreateDate)
            };

            if (!database->RunPrepared(sQuery, params, "Brand::Create"))
                throw std::runtime_error("[RunPrepared exception]");

            if (!UpdateItemBrandsSequence())
                throw std::runtime_error("Error updating ItemBrands sequence");

            database->CommitTransaction();

            return true;
        }
        catch (const std::exception &e)
        {
            database->RollbackTransaction();
            throw(std::runtime_error(std::string("[CreateItemBrand Exception]") +
                                     e.what()));
        }
    }

    bool ItemBrand::Update(
        const omnisphere::dtos::UpdateItemBrand &updateItemBrand) const
    {
        try
        {
            std::vector<std::string> setColumns;
            std::vector<omnisphere::types::SQLParam> parameters;

            if (updateItemBrand.Name.has_value())
            {
                setColumns.push_back("Name");
                parameters.push_back(omnisphere::types::MakeSQLParam(updateItemBrand.Name.value()));
            }

            setColumns.push_back("LastUpdatedBy");
            parameters.push_back(omnisphere::types::MakeSQLParam(updateItemBrand.LastUpdatedBy));

            setColumns.push_back("UpdateDate");
            parameters.push_back(omnisphere::types::MakeSQLParam(updateItemBrand.UpdateDate));

            const std::string sQuery = omnisphere::types::BuildUpdateQuery("Brands", setColumns, "Code = ?");
            parameters.push_back(omnisphere::types::MakeSQLParam(updateItemBrand.Code));

            if (!database->RunPrepared(sQuery, parameters, "ItemBrand::Update"))
                throw std::runtime_error("[RunPrepared exception]");

            database->CommitTransaction();

            return true;
        }
        catch (const std::exception &e)
        {
            database->RollbackTransaction();
            throw(std::runtime_error(std::string("[UpdateItemBrand Exception]") +
                                     e.what()));
        }
    }

    omnisphere::types::DataTable ItemBrand::ReadAll(const std::vector<std::string>& fields) const
    {
        try
        {
            const std::vector<std::string>& selectFields = fields.empty() ? brandSelectFields : fields;
            auto qp = omnisphere::types::BuildQueryParts(selectFields, {});
            const std::string sQuery = "SELECT " + qp.SelectClause + " FROM Brands";

            return database->FetchResults(sQuery, "ItemBrand::ReadAll");
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[ReadAllItemBrands Exception] ") +
                                     " " + e.what());
        }
    }

    omnisphere::types::DataTable
    ItemBrand::Search(const std::vector<std::string>& fields, const omnisphere::dtos::GetItemBrand &itemBrand) const
    {
        try
        {
            std::vector<omnisphere::types::Condition> conditions;
            std::vector<omnisphere::types::SQLParam> parameters;

            if (itemBrand.Entry.has_value())
            {
                conditions.push_back({"", "Entry", "=", "?"});
                parameters.push_back(omnisphere::types::MakeSQLParam(itemBrand.Entry.value()));
            }
            else if (itemBrand.Code.has_value())
            {
                conditions.push_back({"", "Code", "=", "?"});
                parameters.push_back(omnisphere::types::MakeSQLParam(itemBrand.Code.value()));
            }
            else if (itemBrand.Name.has_value())
            {
                conditions.push_back({"", "Name", "LIKE", "?"});
                parameters.push_back(omnisphere::types::MakeSQLParam("%" + itemBrand.Name.value() + "%"));
            }

            const std::vector<std::string>& selectFields = fields.empty() ? brandSelectFields : fields;
            auto qp = omnisphere::types::BuildQueryParts(selectFields, conditions);
            const std::string sQuery = "SELECT " + qp.SelectClause + " FROM Brands" + (qp.WhereClause.empty() ? "" : " WHERE " + qp.WhereClause);

            return database->FetchPrepared(sQuery, parameters, "ItemBrand::Read");
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[ReadItemBrand Exception] ") + " " +
                                     e.what());
        }
    }
} // namespace omnisphere::repositories