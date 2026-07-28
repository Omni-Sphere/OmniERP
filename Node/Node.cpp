#include <Database.hpp>
#include <DataTable.hpp>
#include <DataMapper.hpp>
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

    omnisphere::models::Node
    Node::Add(const std::vector<std::string>& fields, const omnisphere::dtos::CreateNode &node) const
    {
        try
        {
            if (pImpl->nodeRepository->Create(node))
            {
                omnisphere::dtos::GetNode getNode;
                getNode.Code = node.Code;

                return Get(fields, getNode);
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
    Node::Modify(const std::vector<std::string>& fields, const omnisphere::dtos::UpdateNode &node) const
    {
        try
        {
            if (pImpl->nodeRepository->Update(node))
            {
                omnisphere::dtos::GetNode getNode;
                if (node.Entry.has_value())
                {
                    getNode.Entry = node.Entry;
                }
                else if (node.Code.has_value())
                {
                    getNode.Code = node.Code;
                }

                return Get(fields, getNode);
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

    std::vector<omnisphere::models::Node> Node::GetAll(const std::vector<std::string>& fields) const
    {
        try
        {
            std::vector<omnisphere::models::Node> nodes;
            omnisphere::types::DataTable data = pImpl->nodeRepository->ReadAll(fields);

            for (int i = 0; i < data.RowsCount(); i++)
            {
                nodes.push_back(omnisphere::data::MapFromRow<omnisphere::models::Node>(data[i]));
            }

            return nodes;
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[GetAllNodes Exception] ") + e.what());
        }
    }

    omnisphere::models::Node
    Node::Get(const std::vector<std::string>& fields, const omnisphere::dtos::GetNode &getNode) const
    {
        try
        {
            omnisphere::types::DataTable data = pImpl->nodeRepository->Search(fields, getNode);

            if (data.RowsCount() == 0)
                throw std::runtime_error("Node doesn't exist");

            return omnisphere::data::MapFromRow<omnisphere::models::Node>(data[0]);
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("[GetNode Exception] ") + e.what());
        }
    }

    std::vector<omnisphere::models::Node> Node::Search(const std::vector<std::string>& fields, const omnisphere::dtos::GetNode &getNode) const
    {
        try
        {
            std::vector<omnisphere::models::Node> nodes;
            omnisphere::types::DataTable data = pImpl->nodeRepository->Search(fields, getNode);

            for (int i = 0; i < data.RowsCount(); i++)
            {
                nodes.push_back(omnisphere::data::MapFromRow<omnisphere::models::Node>(data[i]));
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
