#include <Database.hpp>
#include <DataTable.hpp>
#include <Database.hpp>
#include <Node/Repositories/Node.hpp>
#include <DataTable.hpp>
#include <Database.hpp>
#include <SQLParams.hpp>
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

    bool NodeRepository::Create(const omnisphere::dtos::CreateNode &node) const
    {
        try
        {
            const std::string query =
                "INSERT INTO Nodes (Entry, Code, Name, NodeType, OperationMode, CashLimit, IPAddress, IsActive, CreatedBy, CreateDate) "
                "VALUES (?, ?, ?, ?, ?, ?, ?, 'Y', ?, ?)";

            std::vector<omnisphere::types::SQLParam> parameters = {
                omnisphere::types::MakeSQLParam(GetCurrentSequence()),
                omnisphere::types::MakeSQLParam(node.Code),
                omnisphere::types::MakeSQLParam(node.Name),
                omnisphere::types::MakeSQLParam(NodeTypeToChar(node.NodeType)),
                omnisphere::types::MakeSQLParam(OperationModeToChar(node.OperationMode)),
                omnisphere::types::MakeSQLParam(node.CashLimit),
                omnisphere::types::MakeSQLParam(node.IPAddress),
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
            std::string query = "UPDATE Nodes SET ";
            std::vector<omnisphere::types::SQLParam> parameters;
            std::vector<std::string> setClauses;

            if (node.Code.has_value())
            {
                setClauses.push_back("Code = ?");
                parameters.push_back(omnisphere::types::MakeSQLParam(node.Code.value()));
            }

            if (node.Name.has_value())
            {
                setClauses.push_back("Name = ?");
                parameters.push_back(omnisphere::types::MakeSQLParam(node.Name.value()));
            }

            if (node.NodeType.has_value())
            {
                setClauses.push_back("NodeType = ?");
                parameters.push_back(omnisphere::types::MakeSQLParam(NodeTypeToChar(node.NodeType.value())));
            }

            if (node.OperationMode.has_value())
            {
                setClauses.push_back("OperationMode = ?");
                parameters.push_back(omnisphere::types::MakeSQLParam(OperationModeToChar(node.OperationMode.value())));
            }

            if (node.CashLimit.has_value())
            {
                setClauses.push_back("CashLimit = ?");
                parameters.push_back(omnisphere::types::MakeSQLParam(node.CashLimit.value()));
            }

            if (node.IPAddress.has_value())
            {
                setClauses.push_back("IPAddress = ?");
                parameters.push_back(omnisphere::types::MakeSQLParam(node.IPAddress.value()));
            }

            if (node.IsActive.has_value())
            {
                setClauses.push_back("IsActive = ?");
                parameters.push_back(omnisphere::types::MakeSQLParam(node.IsActive.value()));
            }

            setClauses.push_back("LastUpdatedBy = ?");
            parameters.push_back(omnisphere::types::MakeSQLParam(node.LastUpdatedBy));

            setClauses.push_back("UpdateDate = ?");
            parameters.push_back(omnisphere::types::MakeSQLParam(node.UpdateDate));

            if (setClauses.empty()) return true;

            for (size_t i = 0; i < setClauses.size(); ++i)
            {
                query += setClauses[i];
                if (i < setClauses.size() - 1) query += ", ";
            }

            query += " WHERE Entry = ?";

            if (node.Entry.has_value())
            {
                parameters.push_back(omnisphere::types::MakeSQLParam(node.Entry.value()));
            }
            else
            {
                throw std::runtime_error("UpdateNode: 'Entry' is required for UPDATE");
            }

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

    omnisphere::types::DataTable NodeRepository::ReadAll() const
    {
        try
        {
            const std::string query =
                "SELECT Entry, Code, Name, NodeType, OperationMode, CashLimit, IPAddress, IsActive, "
                "CreatedBy, CreateDate, LastUpdatedBy, UpdateDate FROM Nodes WHERE IsActive = 'Y'";

            return database->FetchResults(query, "NodeRepository::ReadAll");
        }
        catch (const std::exception &e)
        {
            throw(std::runtime_error(std::string("[ReadAll Exception]") + " " + e.what()));
        }
    }

    omnisphere::types::DataTable NodeRepository::Read(const omnisphere::dtos::GetNode &getNode) const
    {
        try
        {
            std::string query =
                "SELECT Entry, Code, Name, NodeType, OperationMode, CashLimit, IPAddress, IsActive, "
                "CreatedBy, CreateDate, LastUpdatedBy, UpdateDate FROM Nodes WHERE IsActive = 'Y'";
            std::vector<omnisphere::types::SQLParam> parameters;

            if (getNode.Entry.has_value())
            {
                query += " AND Entry = ?";
                parameters.push_back(omnisphere::types::MakeSQLParam(getNode.Entry.value()));
            }
            else if (getNode.Code.has_value())
            {
                query += " AND Code = ?";
                parameters.push_back(omnisphere::types::MakeSQLParam(getNode.Code.value()));
            }
            else if (getNode.Name.has_value())
            {
                query += " AND Name LIKE ?";
                parameters.push_back(omnisphere::types::MakeSQLParam("%" + getNode.Name.value() + "%"));
            }
            else
            {
                throw std::runtime_error("GetNode: 'Entry', 'Code' or 'Name' is required for Read");
            }

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
