#include <Database.hpp>
#include <DataTable.hpp>
#include <SQLParams.hpp>
#include <QueryBuilder.hpp>
#include <Node/Repositories/Node.hpp>
#include <Node/DTOs/CreateNode.hpp>
#include <Node/DTOs/UpdateNode.hpp>
#include <Node/Models/Node.hpp>
#include <stdexcept>
#include <string>
#include <vector>

namespace omnisphere::repositories
{
    NodeRepository::NodeRepository(std::shared_ptr<omnisphere::services::Database> Database) : database(Database) {}

    NodeRepository::~NodeRepository() {}

    // Helper: convert NodeType enum to DB char
    static std::string NodeTypeToChar(omnisphere::enums::NodeType type)
    {
        return type == omnisphere::enums::NodeType::Cashier ? "C" : "S";
    }

    // Helper: convert OperationMode enum to DB char
    static std::string OperationModeToChar(omnisphere::enums::OperationMode mode)
    {
        return mode == omnisphere::enums::OperationMode::POS ? "P" : "R";
    }

    static const std::vector<std::string> nodeSelectFields = {
        "Entry", "Code", "Name", "NodeType", "OperationMode", "CashLimit",
        "IPAddress", "ExtendedLog", "IsActive", "CreatedBy", "CreateDate", "LastUpdatedBy", "UpdateDate"
    };

    bool NodeRepository::Create(const omnisphere::dtos::CreateNode &node) const
    {
        try
        {
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

            if (!database->RunPrepared(query, parameters, "NodeRepository::Create"))
                throw std::runtime_error("[RunPrepared exception]");

            if (!UpdateNodeSequence())
                throw std::runtime_error("[UpdateNodeSequence exception]");

            database->CommitTransaction();

            return true;
        }
        catch (const std::exception &e)
        {
            database->RollbackTransaction();
            throw(std::runtime_error(std::string("[Create Exception]") + " " + e.what()));
        }
    }

    bool NodeRepository::Update(const omnisphere::dtos::UpdateNode &node) const
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

            if (!database->RunPrepared(query, parameters, "NodeRepository::Update"))
                throw std::runtime_error("[RunPrepared exception]");

            database->CommitTransaction();

            return true;
        }
        catch (const std::exception &e)
        {
            database->RollbackTransaction();
            throw(std::runtime_error(std::string("[Update Exception]") + " " + e.what()));
        }
    }

    omnisphere::types::DataTable NodeRepository::ReadAll(const std::vector<std::string>& fields) const
    {
        try
        {
            std::vector<omnisphere::types::Condition> conditions;
            conditions.push_back({"", "IsActive", "=", "'Y'"});

            const std::vector<std::string>& selectFields = fields.empty() ? nodeSelectFields : fields;
            auto qp = omnisphere::types::BuildQueryParts(selectFields, conditions);
            const std::string query = "SELECT " + qp.SelectClause + " FROM Nodes WHERE " + qp.WhereClause;

            return database->FetchResults(query, "NodeRepository::ReadAll");
        }
        catch (const std::exception &e)
        {
            throw(std::runtime_error(std::string("[ReadAll Exception]") + " " + e.what()));
        }
    }

    omnisphere::types::DataTable NodeRepository::Search(const std::vector<std::string>& fields, const omnisphere::dtos::GetNode &getNode) const
    {
        try
        {
            std::vector<omnisphere::types::Condition> conditions;
            conditions.push_back({"", "IsActive", "=", "'Y'"});
            std::vector<omnisphere::types::SQLParam> parameters;

            auto bindFilter = [&](const auto &optField, const std::string &column, const std::string &op, auto transform) {
                if (optField.has_value() && parameters.empty())
                {
                    conditions.push_back({"", column, op, "?"});
                    parameters.push_back(omnisphere::types::MakeSQLParam(transform(optField.value())));
                    return true;
                }
                return false;
            };

            auto pass = [](const auto &val) { return val; };

            bool matched = bindFilter(getNode.Entry, "Entry", "=", pass) ||
                           bindFilter(getNode.Code, "Code", "=", pass) ||
                           bindFilter(getNode.Name, "Name", "LIKE", [](const std::string &s) { return "%" + s + "%"; });

            if (!matched)
            {
                throw std::runtime_error("GetNode: 'Entry', 'Code' or 'Name' is required for Read");
            }

            const std::vector<std::string>& selectFields = fields.empty() ? nodeSelectFields : fields;
            auto qp = omnisphere::types::BuildQueryParts(selectFields, conditions);
            const std::string query = "SELECT " + qp.SelectClause + " FROM Nodes WHERE " + qp.WhereClause;

            return database->FetchPrepared(query, parameters, "NodeRepository::Read");
        }
        catch (const std::exception &e)
        {
            throw(std::runtime_error(std::string("[Read Exception]") + " " + e.what()));
        }
    }

    int NodeRepository::GetCurrentSequence() const
    {
        try
        {
            const std::string query = "SELECT ISNULL(NodeSequence, 0) + 1 NodeSequence FROM Sequences WHERE Entry = 1";

            omnisphere::types::DataTable dataTable = database->FetchResults(query, "NodeRepository::GetCurrentSequence");

            return dataTable[0]["NodeSequence"];
        }
        catch (const std::exception &e)
        {
            throw(std::runtime_error(std::string("[GetCurrentSequence Exception]") + " " + e.what()));
        }
    }

    bool NodeRepository::UpdateNodeSequence() const
    {
        try
        {
            const std::string query = "UPDATE Sequences SET NodeSequence = ISNULL(NodeSequence, 0) + 1";

            if (!database->RunStatement(query, "NodeRepository::UpdateNodeSequence"))
                throw std::runtime_error("[RunStatement exception]");

            return true;
        }
        catch (const std::exception &e)
        {
            throw(std::runtime_error(std::string("[UpdateNodeSequence Exception]") + " " + e.what()));
        }
    }

    bool NodeRepository::Delete(int entry) const
    {
        try
        {
            const std::string query = "UPDATE Nodes SET IsActive = 'N' WHERE Entry = ?";
            std::vector<omnisphere::types::SQLParam> parameters = {
                omnisphere::types::MakeSQLParam(entry)
            };

            if (!database->RunPrepared(query, parameters, "NodeRepository::Delete"))
                throw std::runtime_error("[RunPrepared exception]");

            database->CommitTransaction();

            return true;
        }
        catch (const std::exception &e)
        {
            database->RollbackTransaction();
            throw(std::runtime_error(std::string("[Delete Exception]") + " " + e.what()));
        }
    }
}
