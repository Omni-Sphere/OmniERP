#include <Database.hpp>
#include <DataTable.hpp>
#include <DataTable.hpp>
#include <Database.hpp>
#include <DataTable.hpp>
#include <Node/Node.hpp>
#include <Node/Repositories/Node.hpp>
#include <stdexcept>
#include <string>

namespace omnisphere::node
{
    Node::Node(std::shared_ptr<omnisphere::services::Database> database)
        : pImpl(std::make_unique<Impl>(database)) {}
    Node::~Node() = default;

    struct Node::Impl
    {
        std::shared_ptr<omnisphere::repositories::NodeRepository> nodeRepository;
        explicit Impl(std::shared_ptr<omnisphere::services::Database> database)
            : nodeRepository(
                std::make_shared<omnisphere::repositories::NodeRepository>(
                    database)) {}
    };

    // Helper: convert DB char to NodeType enum
    static omnisphere::enums::NodeType CharToNodeType(const std::string &c)
    {
        return c == "C" ? omnisphere::enums::NodeType::Cashier
                        : omnisphere::enums::NodeType::ServieStation;
    }

    // Helper: convert DB char to OperationMode enum
    static omnisphere::enums::OperationMode CharToOperationMode(const std::string &c)
    {
        return c == "P" ? omnisphere::enums::OperationMode::POS
                        : omnisphere::enums::OperationMode::Restaurant;
    }

    static omnisphere::models::Node MapRow(omnisphere::types::DataTable::Row &row)
    {
        return omnisphere::models::Node(
            row["Entry"],
            row["Code"],
            row["Name"],
            CharToNodeType(static_cast<std::string>(row["NodeType"])),
            CharToOperationMode(static_cast<std::string>(row["OperationMode"])),
            row["CashLimit"].GetOptional<int>(),
            row["IPAddress"].GetOptional<std::string>(),
            static_cast<std::string>(row["IsActive"]) == "Y",
            row["CreatedBy"],
            row["CreateDate"],
            row["LastUpdatedBy"].GetOptional<int>(),
            row["UpdateDate"].GetOptional<std::string>()
        );
    }

    omnisphere::models::Node
    Node::Add(const omnisphere::dtos::CreateNode &node) const
    {
        try
        {
            if (pImpl->nodeRepository->Create(node))
            {
                omnisphere::dtos::GetNode getNode;
                getNode.Code = node.Code;

                return Get(getNode);
            }
            else
            {
                throw std::runtime_error("Error adding node");
            }
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[AddNode Exception] ") + e.what());
        }
    }

    omnisphere::models::Node
    Node::Modify(const omnisphere::dtos::UpdateNode &node) const
    {
        try
        {
            if (pImpl->nodeRepository->Update(node))
            {
                omnisphere::dtos::GetNode getNode;
                getNode.Code = node.Code;

                return Get(getNode);
            }
            else
            {
                throw std::runtime_error("Error updating node");
            }
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[ModifyNode Exception] ") + e.what());
        }
    }

    std::vector<omnisphere::models::Node> Node::GetAll() const
    {
        try
        {
            std::vector<omnisphere::models::Node> nodes;
            omnisphere::types::DataTable data = pImpl->nodeRepository->ReadAll();

            for (int i = 0; i < data.RowsCount(); i++)
            {
                nodes.emplace_back(MapRow(data[i]));
            }

            return nodes;
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[GetAllNodes Exception] ") + e.what());
        }
    }

    omnisphere::models::Node
    Node::Get(const omnisphere::dtos::GetNode &getNode) const
    {
        try
        {
            omnisphere::types::DataTable data = pImpl->nodeRepository->Read(getNode);

            if (data.RowsCount() == 0)
                throw std::runtime_error("Node doesn't exist");

            return MapRow(data[0]);
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[GetNode Exception] ") + e.what());
        }
    }

    std::vector<omnisphere::models::Node> Node::Search(const omnisphere::dtos::GetNode &getNode) const
    {
        try
        {
            std::vector<omnisphere::models::Node> nodes;
            omnisphere::types::DataTable data = pImpl->nodeRepository->Read(getNode);

            for (int i = 0; i < data.RowsCount(); i++)
            {
                nodes.emplace_back(MapRow(data[i]));
            }

            return nodes;
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[SearchNodes Exception] ") + e.what());
        }
    }

    bool Node::Remove(int entry) const
    {
        try
        {
            return pImpl->nodeRepository->Delete(entry);
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[RemoveNode Exception] ") + e.what());
        }
    }
} // namespace omnisphere::node
