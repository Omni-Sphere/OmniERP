#include "Node/DTOs/SearchNode.hpp"
#include <Database.hpp>
#include <DataTable.hpp>
#include <Node/Repositories/Node.hpp>
#include <SQLParams.hpp>
#include <QueryBuilder.hpp>
#include <stdexcept>
#include <string>

#include <DataMapper.hpp>

namespace omnisphere::repositories
{
    static const std::vector<std::string> nodeSelectFields = {
        "Entry", "Code", "Name", "NodeType", "OperationMode", "CashLimit",
        "IPAddress", "ExtendedLog", "IsActive", "CreatedBy", "CreateDate", "LastUpdatedBy", "UpdateDate"
    };

    std::string Node::NodeTypeToChar(omnisphere::enums::NodeType type)
    {
        return type == omnisphere::enums::NodeType::Cashier ? "C" : "S";
    }

    omnisphere::enums::NodeType Node::CharToNodeType(const std::string& str)
    {
        return (str == "C" || str == "Cashier")
            ? omnisphere::enums::NodeType::Cashier
            : omnisphere::enums::NodeType::ServieStation;
    }

    std::string Node::OperationModeToChar(omnisphere::enums::OperationMode mode)
    {
        return mode == omnisphere::enums::OperationMode::POS ? "P" : "R";
    }

    omnisphere::enums::OperationMode Node::CharToOperationMode(const std::string& str)
    {
        if (str == "R" || str == "Restaurant") return omnisphere::enums::OperationMode::Restaurant;
        if (str == "T" || str == "Touch") return omnisphere::enums::OperationMode::Touch;
        return omnisphere::enums::OperationMode::POS;
    }

    omnisphere::models::Node Node::MapNodeRow(omnisphere::types::DataTable::Row& row)
    {
        omnisphere::models::Node n = omnisphere::data::MapFromRow<omnisphere::models::Node>(row);

        if (row.HasColumn("NodeType") && !row["NodeType"].IsNull())
        {
            std::string nodeTypeStr = row["NodeType"];
            n.NodeType = CharToNodeType(nodeTypeStr);
        }

        if (row.HasColumn("OperationMode") && !row["OperationMode"].IsNull())
        {
            std::string opModeStr = row["OperationMode"];
            n.OperationMode = CharToOperationMode(opModeStr);
        }

        return n;
    }

    Node::Node(std::shared_ptr<omnisphere::data::Database> database) : Database(std::move(database)) {}

    Node::~Node() = default;

    bool Node::Create(const omnisphere::dtos::CreateNode &node) const
    {
        try
        {
            Database->BeginTransaction();

            static const std::vector<std::string> insertColumns = {
                "Entry", "Code", "Name", "NodeType", "OperationMode", "CashLimit", "IPAddress", "ExtendedLog", "IsActive", "CreatedBy", "CreateDate"
            };
            const std::string query = omnisphere::types::BuildInsertQuery("Nodes", insertColumns);

            std::string extVal = (node.ExtendedLog.has_value() && node.ExtendedLog.value()) ? "Y" : "N";

            std::vector<omnisphere::types::SQLParam> parameters = {
                omnisphere::types::MakeSQLParam(GetCurrentSequence()),
                omnisphere::types::MakeSQLParam(node.Code),
                omnisphere::types::MakeSQLParam(node.Name),
                omnisphere::types::MakeSQLParam(NodeTypeToChar(node.NodeType)),
                omnisphere::types::MakeSQLParam(OperationModeToChar(node.OperationMode)),
                omnisphere::types::MakeSQLParam(node.CashLimit),
                omnisphere::types::MakeSQLParam(node.IPAddress),
                omnisphere::types::MakeSQLParam(extVal),
                omnisphere::types::MakeSQLParam("Y"),
                omnisphere::types::MakeSQLParam(node.CreatedBy),
                omnisphere::types::MakeSQLParam(node.CreateDate)
            };

            if (!Database->RunPrepared(query, parameters, "Node::Create"))
                throw std::runtime_error("Error creating node");

            if (!UpdateNodeSequence())
                throw std::runtime_error("[UpdateNodeSequence exception]");

            Database->CommitTransaction();

            return true;
        }
        catch (const std::exception &e)
        {
            Database->RollbackTransaction();
            throw std::runtime_error(std::string("[CreateNode Exception] ") + e.what());
        }
    }

    bool Node::Update(const omnisphere::dtos::UpdateNode &node) const
    {
        try
        {
            if (!node.Entry.has_value())
            {
                throw std::runtime_error("UpdateNode: 'Entry' is required for UPDATE");
            }

            std::vector<std::string> setColumns;
            std::vector<omnisphere::types::SQLParam> parameters;

            if (node.Code.has_value())
            {
                setColumns.push_back("Code");
                parameters.push_back(omnisphere::types::MakeSQLParam(node.Code.value()));
            }

            if (node.Name.has_value())
            {
                setColumns.push_back("Name");
                parameters.push_back(omnisphere::types::MakeSQLParam(node.Name.value()));
            }

            if (node.NodeType.has_value())
            {
                setColumns.push_back("NodeType");
                parameters.push_back(omnisphere::types::MakeSQLParam(NodeTypeToChar(node.NodeType.value())));
            }

            if (node.OperationMode.has_value())
            {
                setColumns.push_back("OperationMode");
                parameters.push_back(omnisphere::types::MakeSQLParam(OperationModeToChar(node.OperationMode.value())));
            }

            if (node.CashLimit.has_value())
            {
                setColumns.push_back("CashLimit");
                parameters.push_back(omnisphere::types::MakeSQLParam(node.CashLimit.value()));
            }

            if (node.IPAddress.has_value())
            {
                setColumns.push_back("IPAddress");
                parameters.push_back(omnisphere::types::MakeSQLParam(node.IPAddress.value()));
            }

            if (node.ExtendedLog.has_value())
            {
                setColumns.push_back("ExtendedLog");
                parameters.push_back(omnisphere::types::MakeSQLParam(node.ExtendedLog.value() ? "Y" : "N"));
            }

            if (node.IsActive.has_value())
            {
                setColumns.push_back("IsActive");
                parameters.push_back(omnisphere::types::MakeSQLParam(node.IsActive.value()));
            }

            setColumns.push_back("LastUpdatedBy");
            parameters.push_back(omnisphere::types::MakeSQLParam(node.LastUpdatedBy));

            setColumns.push_back("UpdateDate");
            parameters.push_back(omnisphere::types::MakeSQLParam(node.UpdateDate));

            if (setColumns.empty()) return true;

            const std::string query = omnisphere::types::BuildUpdateQuery("Nodes", setColumns, "[Entry] = ?");
            parameters.push_back(omnisphere::types::MakeSQLParam(node.Entry.value()));

            if (!Database->RunPrepared(query, parameters, "Node::Update"))
                throw std::runtime_error("Error updating node");

            return true;
        }
        catch (const std::exception &e)
        {
            Database->RollbackTransaction();
            throw std::runtime_error(std::string("[UpdateNode Exception] ") + e.what());
        }
    }

    omnisphere::types::DataTable Node::Read(const omnisphere::dtos::GetNode &filter, const std::vector<std::string>& fields) const
    {
        try
        {
            const std::vector<std::string>& selectFields = fields.empty() ? nodeSelectFields : fields;
            std::vector<omnisphere::types::Condition> conditions;
            std::vector<omnisphere::types::SQLParam> params;

            if (filter.Entry.has_value())
            {
                conditions = {{"", "Entry", "=", "?"}};
                params = {omnisphere::types::MakeSQLParam(filter.Entry.value())};
            }
            else if (filter.Code.has_value())
            {
                conditions = {{"", "Code", "=", "?"}};
                params = {omnisphere::types::MakeSQLParam(filter.Code.value())};
            }
            else if (filter.Name.has_value())
            {
                conditions = {{"", "Name", "=", "?"}};
                params = {omnisphere::types::MakeSQLParam(filter.Name.value())};
            }

            auto qp = omnisphere::types::BuildQueryParts(selectFields, conditions);
            std::string sQuery = "SELECT " + qp.SelectClause + " FROM Nodes WHERE " + qp.WhereClause;

            return Database->FetchPrepared(sQuery, params, "Node::Read");
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[ReadNode Exception] ") + e.what());
        }
    }

    omnisphere::types::DataTable Node::ReadAll(const std::vector<std::string>& fields) const
    {
        try
        {
            const std::vector<std::string>& selectFields = fields.empty() ? nodeSelectFields : fields;
            auto qp = omnisphere::types::BuildQueryParts(selectFields, {});
            std::string sQuery = "SELECT " + qp.SelectClause + " FROM Nodes WHERE IsActive = 'Y'";

            return Database->FetchResults(sQuery, "Node::ReadAll");
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[ReadAllNode Exception] ") + e.what());
        }
    }

    omnisphere::types::DataTable Node::Search(const std::vector<std::string>& fields, const omnisphere::dtos::SearchNode &filter) const
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

            const std::vector<std::string>& selectFields = fields.empty() ? nodeSelectFields : fields;
            auto qp = omnisphere::types::BuildQueryParts(selectFields, conditions);

            std::string sQuery = "SELECT " + qp.SelectClause + " FROM Nodes";
            if (!qp.WhereClause.empty())
            {
                sQuery += " WHERE " + qp.WhereClause;
            }

            return Database->FetchPrepared(sQuery, sqlParams, "Node::Search");
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[SearchNode Exception] ") + e.what());
        }
    }

    int Node::GetCurrentSequence() const
    {
        try
        {
            const std::string query = "SELECT COALESCE(NodeSequence, 0) + 1 NodeSequence FROM Sequences WHERE Entry = 1";
            omnisphere::types::DataTable dataTable = Database->FetchResults(query, "Node::GetCurrentSequence");

            return dataTable[0]["NodeSequence"];
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[GetCurrentSequence Exception] ") + e.what());
        }
    }

    bool Node::UpdateNodeSequence() const
    {
        try
        {
            const std::string query = "UPDATE Sequences SET NodeSequence = COALESCE(NodeSequence, 0) + 1";

            if (!Database->RunStatement(query, "Node::UpdateNodeSequence"))
                throw std::runtime_error("[RunStatement exception]");

            return true;
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[UpdateNodeSequence Exception] ") + e.what());
        }
    }

    bool Node::Delete(int entry) const
    {
        try
        {
            const std::string query = "UPDATE Nodes SET IsActive = 'N' WHERE Entry = ?";
            std::vector<omnisphere::types::SQLParam> parameters = {
                omnisphere::types::MakeSQLParam(entry)
            };

            if (!Database->RunPrepared(query, parameters, "Node::Delete"))
                throw std::runtime_error("Error deleting node");

            return true;
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[DeleteNode Exception] ") + e.what());
        }
    }
}
