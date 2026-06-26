#include <Database.hpp>
#include <DataTable.hpp>
#pragma once
#include <vector>

#include <Node/DTOs/CreateNode.hpp>
#include <Node/DTOs/GetNode.hpp>
#include <Node/DTOs/UpdateNode.hpp>
#include <Node/Models/Node.hpp>

#include <Database.hpp>

namespace omnisphere::node
{
    class Node
    {
        struct Impl;
        std::unique_ptr<Impl> pImpl;

        public:
        explicit Node(std::shared_ptr<omnisphere::services::Database> database);

        ~Node();
        omnisphere::models::Node Add(const omnisphere::dtos::CreateNode &node) const;
        omnisphere::models::Node Modify(const omnisphere::dtos::UpdateNode &node) const;
        std::vector<omnisphere::models::Node> GetAll() const;
        omnisphere::models::Node Get(const omnisphere::dtos::GetNode &getNode) const;
        std::vector<omnisphere::models::Node> Search(const omnisphere::dtos::GetNode &getNode) const;
        bool Remove(int entry) const;
    };
} // namespace omnisphere::node
