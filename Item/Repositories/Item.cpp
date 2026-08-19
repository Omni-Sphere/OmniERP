#include "Item/Repositories/Item.hpp"
#include <OmniData/DataTable.hpp>
#include <OmniData/DatabasePool.hpp>
#include <OmniData/QueryBuilder.hpp>
#include <functional>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

namespace omnisphere::repositories {
Item::Item(std::shared_ptr<omnisphere::data::DatabasePool> database)
    : Database(std::move(database)) {}

Item::~Item() = default;

bool Item::Create(const omnisphere::dtos::CreateItem &item) const {
  if (!Database) return false;
  auto conn = Database->Acquire();
  try {
    conn->BeginTransaction();

    auto insertData = omnisphere::types::BuildInsertQuery(
        "Items", GetCurrentSequence(), item);

    if (!conn->RunPrepared(insertData.Query, insertData.Parameters,
                           "Item::Create"))
      throw std::runtime_error("Error creating item");

    UpdateUserSequence();

    conn->CommitTransaction();

    return true;
  } catch (const std::exception &e) {
    conn->RollbackTransaction();
    throw std::runtime_error(std::string("[Create Exception]: ") + e.what());
  }
}

bool Item::Update(const omnisphere::dtos::UpdateItem &item) const {
  if (!Database) return false;
  auto conn = Database->Acquire();
  try {
    auto cols = omnisphere::types::ExtractUpdateColumns(item);

    cols.push_back(
        {"LastUpdatedBy", omnisphere::types::MakeSQLParam(item.LastUpdatedBy)});
    cols.push_back(
        {"UpdateDate", omnisphere::types::MakeSQLParam(item.UpdateDate)});

    auto updateResult = omnisphere::types::BuildUpdateQuery(
        "Items", cols, "Code", omnisphere::types::MakeSQLParam(item.Code));

    if (!conn->RunPrepared(updateResult.Query, updateResult.Parameters,
                           "Item::Update"))
      throw std::runtime_error("Error updating item");

    return true;
  } catch (const std::exception &e) {
    conn->RollbackTransaction();
    throw std::runtime_error(std::string("[Update Exception]: ") + e.what());
  }
}

omnisphere::types::DataTable
Item::Search(const std::vector<std::string> &fields,
             const omnisphere::dtos::ItemFilter &filter) const {
  if (!Database) return {};
  auto conn = Database->Acquire();
  try {
    std::vector<omnisphere::types::Condition> conditions;
    std::vector<omnisphere::types::SQLParam> sqlParams;

    if (filter.Search.has_value()) {
      conditions.push_back({"", "Name", "LIKE", "?"});
      sqlParams.push_back(
          omnisphere::types::MakeSQLParam("%" + filter.Search.value() + "%"));
    }

    if (filter.Code.has_value()) {
      if (filter.Code->eq.has_value()) {
        conditions.push_back({"", "Code", "=", "?"});
        sqlParams.push_back(
            omnisphere::types::MakeSQLParam(filter.Code->eq.value()));
      }
      if (filter.Code->contains.has_value()) {
        conditions.push_back({"", "Code", "LIKE", "?"});
        sqlParams.push_back(omnisphere::types::MakeSQLParam(
            "%" + filter.Code->contains.value() + "%"));
      }
    }

    if (filter.Name.has_value()) {
      if (filter.Name->eq.has_value()) {
        conditions.push_back({"", "Name", "=", "?"});
        sqlParams.push_back(
            omnisphere::types::MakeSQLParam(filter.Name->eq.value()));
      }
      if (filter.Name->contains.has_value()) {
        conditions.push_back({"", "Name", "LIKE", "?"});
        sqlParams.push_back(omnisphere::types::MakeSQLParam(
            "%" + filter.Name->contains.value() + "%"));
      }
    }

    if (filter.IsActive.has_value()) {
      conditions.push_back({"", "IsActive", "=", "?"});
      sqlParams.push_back(
          omnisphere::types::MakeSQLParam(filter.IsActive.value()));
    }

    std::function<void(const std::optional<omnisphere::dtos::FloatFilter> &,
                       const std::string &)>
        mapFloatCondition =
            [&](const std::optional<omnisphere::dtos::FloatFilter> &f,
                const std::string &fieldName) {
              if (f.has_value()) {
                if (f->eq.has_value()) {
                  conditions.push_back({"", fieldName, "=", "?"});
                  sqlParams.push_back(
                      omnisphere::types::MakeSQLParam(f->eq.value()));
                }
                if (f->gt.has_value()) {
                  conditions.push_back({"", fieldName, ">", "?"});
                  sqlParams.push_back(
                      omnisphere::types::MakeSQLParam(f->gt.value()));
                }
                if (f->lt.has_value()) {
                  conditions.push_back({"", fieldName, "<", "?"});
                  sqlParams.push_back(
                      omnisphere::types::MakeSQLParam(f->lt.value()));
                }
                if (f->gte.has_value()) {
                  conditions.push_back({"", fieldName, ">=", "?"});
                  sqlParams.push_back(
                      omnisphere::types::MakeSQLParam(f->gte.value()));
                }
                if (f->lte.has_value()) {
                  conditions.push_back({"", fieldName, "<=", "?"});
                  sqlParams.push_back(
                      omnisphere::types::MakeSQLParam(f->lte.value()));
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

    if (filter.Brand.has_value()) {
      if (filter.Brand->Name.has_value()) {
        if (filter.Brand->Name->eq.has_value()) {
          conditions.push_back({"Brand", "Name", "=", "?"});
          sqlParams.push_back(
              omnisphere::types::MakeSQLParam(filter.Brand->Name->eq.value()));
        }
        if (filter.Brand->Name->contains.has_value()) {
          conditions.push_back({"Brand", "Name", "LIKE", "?"});
          sqlParams.push_back(omnisphere::types::MakeSQLParam(
              "%" + filter.Brand->Name->contains.value() + "%"));
        }
      }
    }

    if (filter.Group.has_value()) {
      if (filter.Group->Name.has_value()) {
        if (filter.Group->Name->eq.has_value()) {
          conditions.push_back({"Group", "Name", "=", "?"});
          sqlParams.push_back(
              omnisphere::types::MakeSQLParam(filter.Group->Name->eq.value()));
        }
        if (filter.Group->Name->contains.has_value()) {
          conditions.push_back({"Group", "Name", "LIKE", "?"});
          sqlParams.push_back(omnisphere::types::MakeSQLParam(
              "%" + filter.Group->Name->contains.value() + "%"));
        }
      }
    }

    static const std::map<std::string, omnisphere::types::RelationMap>
        relations = {{"Brand", {"Brands", "B", "Entry", "I.Brand = B.Entry"}},
                     {"Group", {"Groups", "G", "Entry", "I.Group = G.Entry"}}};

    omnisphere::types::QueryParts queryParts =
        omnisphere::types::BuildQueryParts(fields, conditions, relations, "I");

    std::string sQuery = "SELECT " + queryParts.SelectClause + " FROM Items I" +
                         queryParts.JoinClause;
    if (!queryParts.WhereClause.empty()) {
      sQuery += " WHERE " + queryParts.WhereClause;
    }

    return conn->FetchPrepared(sQuery, sqlParams,
                               "Item::Read(fields, filter)");
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[Read Exception]: ") + e.what());
  }
}

omnisphere::types::DataTable Item::GetByIds(const std::vector<int> &ids) const {
  if (ids.empty() || !Database) return {};
  auto conn = Database->Acquire();
  std::string sQuery = "SELECT Entry, Code, Name, Price, OnHand, IsActive FROM Items WHERE Entry IN (";
  std::vector<omnisphere::types::SQLParam> params;
  for (size_t i = 0; i < ids.size(); ++i) {
    if (i > 0) sQuery += ", ";
    sQuery += "?";
    params.push_back(omnisphere::types::MakeSQLParam(ids[i]));
  }
  sQuery += ")";
  return conn->FetchPrepared(sQuery, params);
}

ItemCursorPage Item::GetPage(std::optional<int> afterEntry, int limit) const {
  if (!Database) return {};
  auto conn = Database->Acquire();

  std::string countQuery = "SELECT COALESCE(COUNT(*), 0) AS Total FROM Items";
  auto totalTable = conn->FetchResults(countQuery);
  int totalCount = 0;
  if (totalTable.RowsCount() > 0) {
    totalCount = totalTable[0]["Total"];
  }

  std::string sQuery;
  std::vector<omnisphere::types::SQLParam> params;

  if (afterEntry.has_value()) {
    sQuery = "SELECT Entry, Code, Name, Price, OnHand, IsActive FROM Items WHERE Entry > ? ORDER BY Entry ASC LIMIT ?";
    params.push_back(omnisphere::types::MakeSQLParam(afterEntry.value()));
    params.push_back(omnisphere::types::MakeSQLParam(limit + 1));
  } else {
    sQuery = "SELECT Entry, Code, Name, Price, OnHand, IsActive FROM Items ORDER BY Entry ASC LIMIT ?";
    params.push_back(omnisphere::types::MakeSQLParam(limit + 1));
  }

  auto table = conn->FetchPrepared(sQuery, params);
  ItemCursorPage page;
  page.dataTable = table;
  page.totalCount = totalCount;
  page.hasPreviousPage = afterEntry.has_value();

  if (table.RowsCount() > static_cast<size_t>(limit)) {
    page.nextCursor = table[limit - 1]["Entry"];
  }

  return page;
}

int Item::GetCurrentSequence() const {
  if (!Database) return 0;
  auto conn = Database->Acquire();
  try {
    const std::string sQuery = "SELECT COALESCE(ItemSequence, 0) + 1 "
                               "ItemSequence FROM Sequences WHERE Entry = 1";

    omnisphere::types::DataTable data =
        conn->FetchResults(sQuery, "Item::GetCurrentSequence");

    if (data.RowsCount() == 1)
      return data[0]["ItemSequence"];
    else
      return 0;
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[GetCurrentSequence Exception] ") +
                             " " + e.what());
  }
}

bool Item::UpdateUserSequence() const {
  if (!Database) return false;
  auto conn = Database->Acquire();
  try {
    const std::string sQuery =
        "UPDATE Sequences SET ItemSequence = COALESCE(ItemSequence,0) + 1";

    if (!conn->RunStatement(sQuery, "Item::UpdateUserSequence"))
      return false;

    return true;
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("[UpdateUserSequence Exception] ") +
                             " " + e.what());
  }
}
} // namespace omnisphere::repositories