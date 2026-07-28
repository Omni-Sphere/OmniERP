#include <Database.hpp>
#include <DataTable.hpp>
#pragma once
#include <DataTable.hpp>

#include <Node/DTOs/CreateNode.hpp>
#include <Node/DTOs/GetNode.hpp>
#include <Node/DTOs/UpdateNode.hpp>
#include <Database.hpp>
#include <memory>
#include <vector>

namespace omnisphere::repositories
{
    class NodeRepository
    {
        private:
        std::shared_ptr<omnisphere::services::Database> database;

        public:
        explicit NodeRepository(std::shared_ptr<omnisphere::services::Database> Database);
        ~NodeRepository();
        bool UpdateNodeSequence() const;
        int GetCurrentSequence() const;
        bool Create(const omnisphere::dtos::CreateNode &node) const;
        bool Update(const omnisphere::dtos::UpdateNode &node) const;
        omnisphere::types::DataTable ReadAll(const std::vector<std::string>& fields = {}) const;
        omnisphere::types::DataTable Search(const std::vector<std::string>& fields, const omnisphere::dtos::GetNode &getNode) const;
        bool Delete(int entry) const;
    };
};
