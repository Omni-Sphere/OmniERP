#include <Database.hpp>
#include <DataTable.hpp>
#include <QueryBuilder.hpp>
#include <ItemGroup/Repositories/ItemGroup.hpp>

namespace omnisphere::repositories
{
    static const std::vector<std::string> groupSelectFields = {
        "Entry", "Code", "Name", "CreatedBy", "CreateDate", "LastUpdatedBy", "UpdateDate"
    };

    ItemGroup::ItemGroup(std::shared_ptr<omnisphere::services::Database> Database)
        : database(std::move(Database)) {}

    int ItemGroup::GetCurrentSequence() const
    {
        try
        {
            const std::string sQuery =
            "SELECT ISNULL(ItemGroupsSequence, 0) + 1 ItemGroupsSequence FROM "
            "Sequences WHERE Entry = 1";

            omnisphere::types::DataTable data = database->FetchResults(sQuery, "ItemGroup::GetCurrentSequence");

            if (data.RowsCount() == 1)
                return data[0]["ItemGroupsSequence"];
            else
                return 0;
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[GetCurrentSequence Exception] ") +
                                     " " + e.what());
        }
    }

    bool ItemGroup::UpdateItemGroupsSequence() const
    {
        try
        {
            const std::string sQuery = "UPDATE Sequences SET ItemGroupsSequence = "
            "ISNULL(ItemGroupsSequence,0) + 1";

            if (!database->RunStatement(sQuery, "ItemGroup::UpdateItemGroupsSequence"))
                return false;

            return true;
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[ItemGroupsSequence Exception] ") +
                                     " " + e.what());
        }
    }

    bool ItemGroup::Create(
        const omnisphere::dtos::CreateItemGroup &createItemGroup) const
    {
        try
        {
            static const std::vector<std::string> insertColumns = {
                "Entry", "Code", "Name", "CreatedBy", "CreateDate"
            };
            const std::string sQuery = omnisphere::types::BuildInsertQuery("Groups", insertColumns);

            std::vector<omnisphere::types::SQLParam> params = {
                omnisphere::types::MakeSQLParam(GetCurrentSequence()),
                omnisphere::types::MakeSQLParam(createItemGroup.Code),
                omnisphere::types::MakeSQLParam(createItemGroup.Name),
                omnisphere::types::MakeSQLParam(createItemGroup.CreatedBy),
                omnisphere::types::MakeSQLParam(createItemGroup.CreateDate)
            };

            if (!database->RunPrepared(sQuery, params, "ItemGroup::Create"))
                throw std::runtime_error("[RunPrepared exception]");

            if (!UpdateItemGroupsSequence())
                throw std::runtime_error("Error updating item group sequence");

            database->CommitTransaction();

            return true;
        }
        catch (const std::exception &e)
        {
            database->RollbackTransaction();
            throw(std::runtime_error(std::string("[CreateItemGroup Exception]") +
                                     e.what()));
        }
    }

    bool ItemGroup::Update(
        const omnisphere::dtos::UpdateItemGroup &updateItemGroup) const
    {
        try
        {
            std::vector<std::string> setColumns;
            std::vector<omnisphere::types::SQLParam> parameters;

            if (updateItemGroup.Name.has_value())
            {
                setColumns.push_back("Name");
                parameters.push_back(omnisphere::types::MakeSQLParam(updateItemGroup.Name.value()));
            }

            setColumns.push_back("LastUpdatedBy");
            parameters.push_back(omnisphere::types::MakeSQLParam(updateItemGroup.LastUpdatedBy));

            setColumns.push_back("UpdateDate");
            parameters.push_back(omnisphere::types::MakeSQLParam(updateItemGroup.UpdateDate));

            const std::string sQuery = omnisphere::types::BuildUpdateQuery("Groups", setColumns, "Code = ?");
            parameters.push_back(omnisphere::types::MakeSQLParam(updateItemGroup.Code));

            if (!database->RunPrepared(sQuery, parameters, "ItemGroup::Update"))
                throw std::runtime_error("[RunPrepared exception]");

            database->CommitTransaction();

            return true;
        }
        catch (const std::exception &e)
        {
            database->RollbackTransaction();
            throw(std::runtime_error(std::string("[UpdateItemGroup Exception]") +
                                     e.what()));
        }
    }

    omnisphere::types::DataTable ItemGroup::ReadAll(const std::vector<std::string>& fields) const
    {
        try
        {
            const std::vector<std::string>& selectFields = fields.empty() ? groupSelectFields : fields;
            auto qp = omnisphere::types::BuildQueryParts(selectFields, {});
            const std::string sQuery = "SELECT " + qp.SelectClause + " FROM Groups";

            return database->FetchResults(sQuery, "ItemGroup::ReadAll");
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[ReadAllItemGroups Exception] ") +
                                     e.what());
        }
    }

    omnisphere::types::DataTable
    ItemGroup::Search(const std::vector<std::string>& fields, const omnisphere::dtos::GetItemGroup &itemGroup) const
    {
        try
        {
            std::vector<omnisphere::types::Condition> conditions;
            std::vector<omnisphere::types::SQLParam> parameters;

            if (itemGroup.Entry.has_value())
            {
                conditions.push_back({"", "Entry", "=", "?"});
                parameters.push_back(omnisphere::types::MakeSQLParam(itemGroup.Entry.value()));
            }
            else if (itemGroup.Code.has_value())
            {
                conditions.push_back({"", "Code", "=", "?"});
                parameters.push_back(omnisphere::types::MakeSQLParam(itemGroup.Code.value()));
            }
            else if (itemGroup.Name.has_value())
            {
                conditions.push_back({"", "Name", "LIKE", "?"});
                parameters.push_back(omnisphere::types::MakeSQLParam("%" + itemGroup.Name.value() + "%"));
            }
            else
            {
                throw std::runtime_error("At least one filter must be provided to read ItemGroup");
            }

            const std::vector<std::string>& selectFields = fields.empty() ? groupSelectFields : fields;
            auto qp = omnisphere::types::BuildQueryParts(selectFields, conditions);
            const std::string sQuery = "SELECT " + qp.SelectClause + " FROM Groups WHERE " + qp.WhereClause;

            return database->FetchPrepared(sQuery, parameters, "ItemGroup::Read");
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[ReadItemGroup Exception] ") +
                                     e.what());
        }
    }
} // namespace omnisphere::repositories