#include <Database.hpp>
#include <DataTable.hpp>
#include <DataTable.hpp>
#include <Database.hpp>
#include <DataTable.hpp>
#include <Item/Repositories/Item.hpp>
#include <map>
#include <string>
#include <vector>
#include <functional>
#include <QueryBuilder.hpp>


namespace omnisphere::repositories
{
    Item::Item(std::shared_ptr<omnisphere::services::Database> database)
        : Database(std::move(database)) {}

    Item::~Item() = default;

    bool Item::Create(const omnisphere::dtos::CreateItem &item) const
    {
        try
        {
            Database->BeginTransaction();

            std::string sQuery =
            "INSERT INTO Items ("
            "[Entry], "
            "[Code], "
            "[Name], "
            "Description, "
            "Image, "
            "PurchaseItem, "
            "SellItem, "
            "InventoryItem, "
            "Price, "
            "Brand, "
            "[Group], "
            "MinStock, "
            "MaxStock, "
            "MinOrder, "
            "MaxOrder, "
            "MinRequest, "
            "MaxRequest, "
            "CreatedBy, "
            "CreateDate"
            ")"
            "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";

            const std::vector<omnisphere::types::SQLParam> params = {
                omnisphere::types::MakeSQLParam(GetCurrentSequence()),
                omnisphere::types::MakeSQLParam(item.Code),
                omnisphere::types::MakeSQLParam(item.Name),
                omnisphere::types::MakeSQLParam(item.Description),
                omnisphere::types::MakeSQLParam(item.Image),
                omnisphere::types::MakeSQLParam(item.PurchaseItem),
                omnisphere::types::MakeSQLParam(item.SellItem),
                omnisphere::types::MakeSQLParam(item.InventoryItem),
                omnisphere::types::MakeSQLParam(item.Price),
                omnisphere::types::MakeSQLParam(item.Brand),
                omnisphere::types::MakeSQLParam(item.Group),
                omnisphere::types::MakeSQLParam(item.MinStock),
                omnisphere::types::MakeSQLParam(item.MaxStock),
                omnisphere::types::MakeSQLParam(item.MinOrder),
                omnisphere::types::MakeSQLParam(item.MaxOrder),
                omnisphere::types::MakeSQLParam(item.MinRequest),
                omnisphere::types::MakeSQLParam(item.MaxRequest),
                omnisphere::types::MakeSQLParam(item.CreatedBy),
                omnisphere::types::MakeSQLParam(item.CreateDate)};

            if (!Database->RunPrepared(sQuery, params, "Item::Create"))
                throw std::runtime_error("Error creating item");

            UpdateUserSequence();

            Database->CommitTransaction();

            return true;
        }
        catch (const std::exception &e)
        {
            Database->RollbackTransaction();
            throw std::runtime_error(std::string("[Create Exception]: ") + e.what());
        }
    }

    bool Item::Update(const omnisphere::dtos::UpdateItem &item) const
    {
        try
        {
            std::string sQuery = "UPDATE Items SET ";
            std::vector<omnisphere::types::SQLParam> params;

            if (item.Name.has_value())
            {
                sQuery += "Name = ?, ";
                params.emplace_back(omnisphere::types::MakeSQLParam(item.Name.value()));
            }

            if (item.Description.has_value())
            {
                sQuery += "Description = ?, ";
                params.emplace_back(
                    omnisphere::types::MakeSQLParam(item.Description.value()));
            }

            if (item.Image.has_value())
            {
                sQuery += "Image = ?, ";
                params.emplace_back(omnisphere::types::MakeSQLParam(item.Image.value()));
            }

            if (item.PurchaseItem.has_value())
            {
                sQuery += "PurchaseItem = ?, ";
                params.emplace_back(
                    omnisphere::types::MakeSQLParam(item.PurchaseItem.value()));
            }

            if (item.SellItem.has_value())
            {
                sQuery += "SellItem = ?, ";
                params.emplace_back(
                    omnisphere::types::MakeSQLParam(item.SellItem.value()));
            }

            if (item.InventoryItem.has_value())
            {
                sQuery += "InventoryItem = ?, ";
                params.emplace_back(
                    omnisphere::types::MakeSQLParam(item.InventoryItem.value()));
            }

            if (item.Price.has_value())
            {
                sQuery += "Price = ?, ";
                params.emplace_back(omnisphere::types::MakeSQLParam(item.Price.value()));
            }

            if (item.Brand.has_value())
            {
                sQuery += "Brand = ?, ";
                params.emplace_back(omnisphere::types::MakeSQLParam(item.Brand.value()));
            }

            if (item.Group.has_value())
            {
                sQuery += "Group = ?, ";
                params.emplace_back(omnisphere::types::MakeSQLParam(item.Group.value()));
            }

            sQuery += "WHERE Entry = ?";

            if (!Database->RunPrepared(sQuery, params, "Item::Update"))
                throw std::runtime_error("Error updating item");

            return true;
        }
        catch (const std::exception &e)
        {
            Database->RollbackTransaction();
            throw std::runtime_error(std::string("[Update Exception]: ") + e.what());
        }
    }

    omnisphere::types::DataTable
    Item::Read(const std::vector<std::string>& fields, const omnisphere::dtos::ItemFilter &filter) const
    {
        try
        {
            std::vector<omnisphere::types::Condition> conditions;
            std::vector<omnisphere::types::SQLParam> sqlParams;

            // Search (Global text search, skipped for now or simple implementation)
            if (filter.Search.has_value()) {
                conditions.push_back({"", "Name", "LIKE", "?"});
                sqlParams.push_back(omnisphere::types::MakeSQLParam("%" + filter.Search.value() + "%"));
            }

            // Code
            if (filter.Code.has_value()) {
                if (filter.Code->eq.has_value()) {
                    conditions.push_back({"", "Code", "=", "?"});
                    sqlParams.push_back(omnisphere::types::MakeSQLParam(filter.Code->eq.value()));
                }
                if (filter.Code->contains.has_value()) {
                    conditions.push_back({"", "Code", "LIKE", "?"});
                    sqlParams.push_back(omnisphere::types::MakeSQLParam("%" + filter.Code->contains.value() + "%"));
                }
            }

            // Name
            if (filter.Name.has_value()) {
                if (filter.Name->eq.has_value()) {
                    conditions.push_back({"", "Name", "=", "?"});
                    sqlParams.push_back(omnisphere::types::MakeSQLParam(filter.Name->eq.value()));
                }
                if (filter.Name->contains.has_value()) {
                    conditions.push_back({"", "Name", "LIKE", "?"});
                    sqlParams.push_back(omnisphere::types::MakeSQLParam("%" + filter.Name->contains.value() + "%"));
                }
            }

            if (filter.IsActive.has_value()) {
                conditions.push_back({"", "IsActive", "=", "?"});
                sqlParams.push_back(omnisphere::types::MakeSQLParam(filter.IsActive.value()));
            }

            // Price Advanced Filters
            std::function<void(const std::optional<omnisphere::dtos::FloatFilter>&, const std::string&)> mapFloatCondition = [&](const std::optional<omnisphere::dtos::FloatFilter>& f, const std::string& fieldName) {
                if (f.has_value()) {
                    if (f->eq.has_value()) {
                        conditions.push_back({"", fieldName, "=", "?"});
                        sqlParams.push_back(omnisphere::types::MakeSQLParam(f->eq.value()));
                    }
                    if (f->gt.has_value()) {
                        conditions.push_back({"", fieldName, ">", "?"});
                        sqlParams.push_back(omnisphere::types::MakeSQLParam(f->gt.value()));
                    }
                    if (f->lt.has_value()) {
                        conditions.push_back({"", fieldName, "<", "?"});
                        sqlParams.push_back(omnisphere::types::MakeSQLParam(f->lt.value()));
                    }
                    if (f->gte.has_value()) {
                        conditions.push_back({"", fieldName, ">=", "?"});
                        sqlParams.push_back(omnisphere::types::MakeSQLParam(f->gte.value()));
                    }
                    if (f->lte.has_value()) {
                        conditions.push_back({"", fieldName, "<=", "?"});
                        sqlParams.push_back(omnisphere::types::MakeSQLParam(f->lte.value()));
                    }
                }
            };

            mapFloatCondition(filter.Price, "Price");
            mapFloatCondition(filter.OnHand, "OnHand");
            mapFloatCondition(filter.OnOrder, "OnOrder");
            mapFloatCondition(filter.OnRequest, "OnRequest");
            mapFloatCondition(filter.MinStock, "MinStock");
            mapFloatCondition(filter.MaxStock, "MaxStock");
            mapFloatCondition(filter.MinOrder, "MinOrder");
            mapFloatCondition(filter.MaxOrder, "MaxOrder");
            mapFloatCondition(filter.MinRequest, "MinRequest");
            mapFloatCondition(filter.MaxRequest, "MaxRequest");


            // Brand
            if (filter.Brand.has_value()) {
                if (filter.Brand->Name.has_value()) {
                    if (filter.Brand->Name->eq.has_value()) {
                        conditions.push_back({"Brand", "Name", "=", "?"});
                        sqlParams.push_back(omnisphere::types::MakeSQLParam(filter.Brand->Name->eq.value()));
                    }
                    if (filter.Brand->Name->contains.has_value()) {
                        conditions.push_back({"Brand", "Name", "LIKE", "?"});
                        sqlParams.push_back(omnisphere::types::MakeSQLParam("%" + filter.Brand->Name->contains.value() + "%"));
                    }
                }
            }

            // Group
            if (filter.Group.has_value()) {
                if (filter.Group->Name.has_value()) {
                    if (filter.Group->Name->eq.has_value()) {
                        conditions.push_back({"Group", "Name", "=", "?"});
                        sqlParams.push_back(omnisphere::types::MakeSQLParam(filter.Group->Name->eq.value()));
                    }
                    if (filter.Group->Name->contains.has_value()) {
                        conditions.push_back({"Group", "Name", "LIKE", "?"});
                        sqlParams.push_back(omnisphere::types::MakeSQLParam("%" + filter.Group->Name->contains.value() + "%"));
                    }
                }
            }

            static const std::map<std::string, omnisphere::types::RelationMap> relations = {
                {"Brand", {"Brands", "B", "Entry", "I.[Brand] = B.[Entry]"}},
                {"Group", {"Groups", "G", "Entry", "I.[Group] = G.[Entry]"}}
            };

            omnisphere::types::QueryParts queryParts = omnisphere::types::BuildQueryParts(fields, conditions, relations, "I");

            std::string sQuery = "SELECT " + queryParts.SelectClause + " FROM Items I" + queryParts.JoinClause;
            if (!queryParts.WhereClause.empty()) {
                sQuery += " WHERE " + queryParts.WhereClause;
            }

            return Database->FetchPrepared(sQuery, sqlParams, "Item::Read(fields, filter)");
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[Read Exception]: ") + e.what());
        }
    }

    int Item::GetCurrentSequence() const
    {
        try
        {
            const std::string sQuery = "SELECT ISNULL(ItemSequence, 0) + 1 "
            "ItemSequence FROM Sequences WHERE Entry = 1";

            omnisphere::types::DataTable data = Database->FetchResults(sQuery, "Item::GetCurrentSequence");

            if (data.RowsCount() == 1)
                return data[0]["ItemSequence"];
            else
                return 0;
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[GetCurrentSequence Exception] ") +
                                     " " + e.what());
        }
    };

    bool Item::UpdateUserSequence() const
    {
        try
        {
            const std::string sQuery =
            "UPDATE Sequences SET ItemSequence = ISNULL(ItemSequence,0) + 1";

            if (!Database->RunStatement(sQuery, "Item::UpdateUserSequence"))
                return false;

            return true;
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[UpdateUserSequence Exception] ") +
                                     " " + e.what());
        }
    };
}; // namespace omnisphere::repositories